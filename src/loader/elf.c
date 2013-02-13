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
#include <elf.h>

static inline int is_elf(Elf32_Ehdr *header)
{
	return  header->e_ident[EI_MAG0] == ELFMAG0 &&
		header->e_ident[EI_MAG1] == ELFMAG1 &&
		header->e_ident[EI_MAG2] == ELFMAG2 &&
		header->e_ident[EI_MAG3] == ELFMAG3;
}

int elf_load(struct ksim_binary *binary)
{
	Elf32_Ehdr *header = (Elf32_Ehdr *)binary->base;
	
	if (binary->size < sizeof(*header))
		return -1;
	
	if (!is_elf(header))
		return -1;
	
	return -1;
}
