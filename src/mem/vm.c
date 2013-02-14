/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * mem/vm.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-mem.h>
#include <ksim-thread.h>
#include <malloc.h>

/* -----------------------------------------------------------------------------
 * VIRTUAL (APPLICATION) MEMORY -> PHYSICAL (EMULATED MACHINE) MEMORY
 * -----------------------------------------------------------------------------
 * Which system is responsible for which part?
 * 
 * How about some kind of hybrid system.  So, ArcSim creates a VM area - which
 * is fully accessible at all times to the executing program.  But, ksim
 * allocates memory within that vma.
 * 
 * What about in the future when you want to implement an MMU?
 * 
 * Let's ensure that the interface supports both modes of operation:
 *   + MMU
 *   + NOMMU
 */
static inline int is_allocated(struct ksim_context *ctx, void __guest *addr)
{
	struct ksim_vm_info *vmi = thread_current(ctx)->vm;
	struct vm_alloc_region *rgn;
		
	for (rgn = vmi->regions; rgn; rgn = rgn->next) {
		if ((unsigned long)addr >= rgn->base && (unsigned long)addr <= rgn->base + rgn->size)
			return 1;
	}
	
	return 0;
}

struct ksim_vm_info *vm_create_info(struct ksim_context *ctx)
{
	struct ksim_vm_info *vmi;
	
	vmi = malloc(sizeof(*vmi));
	if (!vmi) {
		return NULL;
	}
	
	vmi->regions = NULL;
	
	return vmi;
}

int vm_alloc_fixed(struct ksim_context *ctx, void __guest *addr, unsigned int size)
{
	struct ksim_vm_info *vmi = thread_current(ctx)->vm;
	struct vm_alloc_region *rgn;
	
	/* Must be aligned to 4-byte boundary. */
	/* TODO: Should this be 8-bytes for 64-bit emulation? */
	if ((unsigned long)addr % 4)
		return -1;
	
	/* Size must be aligned to page size. */
	if (size % GUEST_PAGE_SIZE)
		size += GUEST_PAGE_SIZE - (size % GUEST_PAGE_SIZE);
	
	/* Look for overlapping allocation regions, and instantly refuse
	 * allocation. */
	if (is_allocated(ctx, addr)) {
		kdbg("vm: address already allocated\n");
		return -1;
	}
	
	/* Allocate storage for the allocation region descriptor. */
	rgn = malloc(sizeof(*rgn));
	if (!rgn) {
		return -1;
	}
	
	/* Populate the region descriptor, and insert it into the list. */
	rgn->base = (unsigned long)addr;
	rgn->size = size;
	rgn->next = vmi->regions;
	
	vmi->regions = rgn;
	
	kdbg("vm: alloc: base=0x%lx, size=0x%x\n", rgn->base, rgn->size);
	
	return 0;
}

void __guest *vm_alloc(struct ksim_context *ctx, unsigned int size)
{
	void *addr = (void *)GUEST_HEAP_BASE;
	
	/* TODO:  Handle OOM condition. */
	while (!vm_alloc_fixed(ctx, addr, size)) {
		addr = (void *)((unsigned long)addr + GUEST_PAGE_SIZE);
	}
	
	return addr;
}

void vm_free(struct ksim_context *ctx, void __guest *addr)
{
	struct ksim_vm_info *vmi = thread_current(ctx)->vm;
	struct vm_alloc_region *rgn;
		
	for (rgn = vmi->regions; rgn; rgn = rgn->next) {
		if ((unsigned long)addr == rgn->base) {
			/* TODO: Remove from list. */
			kdbg("vm: free region - not implemented\n");
			break;
		}
	}
	
	kdbg("vm: attempt to free region without matching base pointer\n");
}

int vm_copy_to(struct ksim_context *ctx, void __guest *addr, void *src, unsigned int size)
{
	if (!is_allocated(ctx, addr))
		return -1;
	
	return ctx->arcsim->vm_write(src, (unsigned long)addr, size);
}

int vm_copy_from(struct ksim_context *ctx, void __guest *addr, void *dest, unsigned int size)
{
	if (!is_allocated(ctx, addr))
		return -1;

	return ctx->arcsim->vm_write(dest, (unsigned long)addr, size);
}

char *vm_read_string(struct ksim_context *ctx, const char __guest *addr, unsigned int size)
{
	char *buffer = calloc(size + 1, 1);
	int rc;
	
	rc = vm_copy_from(ctx, (void *)addr, buffer, size);
	if (rc) {
		free(buffer);
		return NULL;
	}
	
	return buffer;
}

void vm_free_string(struct ksim_context *ctx, char *str)
{
	free(str);
}
