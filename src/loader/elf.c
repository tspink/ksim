/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * loader/elf.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-binary.h>
#include <malloc.h>
#include <string.h>
#include <elf.h>
#include <ksim-mem.h>
#include <ksim-thread.h>

struct ksim_elf_info {
	int tag;
};

#define MIN_PH_ENT 1
#define MAX_PH_ENT (65536U / sizeof(Elf32_Phdr))

static inline int is_elf(Elf32_Ehdr *header)
{
	return  header->e_ident[EI_MAG0] == ELFMAG0 &&
		header->e_ident[EI_MAG1] == ELFMAG1 &&
		header->e_ident[EI_MAG2] == ELFMAG2 &&
		header->e_ident[EI_MAG3] == ELFMAG3;
}

static inline int check_arch(struct ksim_context *ctx, Elf32_Ehdr *header)
{
	/* TODO: Flesh this out as architectures are added. */
	switch (header->e_machine) {
	case EM_ARM:
		return ctx->opt->arch_type == ARCH_ARM;
	default:
		return 0;
	}
}

static int load_segment(struct ksim_binary *binary, Elf32_Phdr *prog_header, unsigned int load_bias)
{
	void __guest *load_address = (void __guest *)((unsigned long)prog_header->p_vaddr + load_bias);
	void *data = (void *)((unsigned long)binary->base + prog_header->p_offset);
	int rc;
	
	rc = vm_alloc_fixed(binary->ctx, load_address, prog_header->p_filesz);
	if (rc) {
		return -1;
	}
	
	return vm_copy_to(binary->ctx, load_address, data, prog_header->p_filesz);
}

static int load_program_header(struct ksim_binary *binary, struct ksim_elf_info *elf_info, Elf32_Ehdr *elf_header)
{
	Elf32_Phdr *prog_header = (Elf32_Phdr *)((unsigned long)elf_header + elf_header->e_phoff);
	char *elf_interpreter;
	int i, rc;
	
	if (elf_header->e_phentsize != sizeof(Elf32_Phdr)) {
		kdbg("elf: incompatible program header\n");
		return -1;
	}
	
	if (elf_header->e_phnum < MIN_PH_ENT || elf_header->e_phnum > MAX_PH_ENT) {
		kdbg("elf: program header count out of range\n");
		return -1;
	}

	for (i = 0; i < elf_header->e_phnum; i++) {
		if (prog_header[i].p_type == PT_INTERP) {
			kdbg("elf: found interpreter\n");
			elf_interpreter = calloc(1, prog_header[i].p_filesz + 1);
			if (!elf_interpreter) {
				kdbg("elf: unable to allocate memory for ELF interpreter filename\n");
				return -1;
			}
			
			strncpy(elf_interpreter, (char *)elf_header + prog_header[i].p_offset, prog_header[i].p_filesz);
			kdbg("elf: interpreter: %s\n", elf_interpreter);
			kdbg("elf: loading interpreter indirectly not supported.\n");
			free(elf_interpreter);
			return -1;
		} else if (prog_header[i].p_type == PT_LOAD) {
			kdbg("elf: found loadable\n");
			rc = load_segment(binary, &prog_header[i], 0);
			if (rc) {
				kdbg("elf: load segment failed\n");
				return -1;
			}
		}
	}
	
	return 0;
}

static int initial_sanity_check(struct ksim_binary *binary, Elf32_Ehdr *header)
{
	/* Quick sanity check on file size. */
	if (binary->size < sizeof(*header)) {
		kdbg("elf: binary size invalid\n");
		return -1;
	}
	
	/* Check that the file is indeed an ELF file. */
	if (!is_elf(header)) {
		kdbg("elf: magic number invalid\n");
		return -1;
	}
	
	/* Check that the ELF file is an executable. */
	if (header->e_type != ET_EXEC && header->e_type != ET_DYN) {
		kdbg("elf: not an executable file\n");
		return -1;
	}
	
	/* Check that the architecture of the ELF file matches the
	 * architecture we're emulating. */
	if (!check_arch(binary->ctx, header)) {
		kdbg("elf: invalid architecture\n");
		return -1;
	}
	
	return 0;
}

static int init_stack(struct ksim_binary *binary)
{
	struct ksim_vm_info *vmi = thread_current(binary->ctx)->vm;
	static const int stack_size = 0x4000;
	
	/* Allocate stack area */
	return vm_alloc_fixed(binary->ctx, (void *)(vmi->stack_bottom - stack_size), stack_size);
}

static int init_heap(struct ksim_binary *binary)
{
	struct ksim_vm_info *vmi = thread_current(binary->ctx)->vm;
	static const int heap_size = 0x4000;
	
	/* Allocate heap area */
	return vm_alloc_fixed(binary->ctx, (void *)vmi->heap_base, heap_size);
}

int elf_load(struct ksim_binary *binary)
{
	Elf32_Ehdr *header = (Elf32_Ehdr *)binary->base;
	struct ksim_vm_info *vmi = thread_current(binary->ctx)->vm;
	struct ksim_elf_info *elf_info;
	int rc;
	
	kdbg("elf: attempting to load binary\n");
	rc = initial_sanity_check(binary, header);
	if (rc)
		return rc;
	
	elf_info = malloc(sizeof(*elf_info));
	if (!elf_info) {
		kdbg("elf: out of memory, whilst allocating storage for elf_info\n");
		return -1;
	}
	
	binary->priv = elf_info;	
	binary->entry_point = header->e_entry;
	
	/* Setup the program's VM information. */
	vmi->stack_bottom = 0x40000000;
	vmi->heap_base = 0x40000000;

	/* Initialise the program stack. */
	rc = init_stack(binary);
	if (rc) {
		free(elf_info);
		return rc;
	}
	
	/* Initialise the program heap. */
	rc = init_heap(binary);
	if (rc) {
		free(elf_info);
		return rc;
	}

	/* Now, load the program into memory. */
	rc = load_program_header(binary, elf_info, header);
	if (rc) {
		free(elf_info);
		return rc;
	}
	
	return 0;
}
