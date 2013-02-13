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

void *vm_alloc_fixed(struct ksim_context *ctx, void *addr, unsigned int size)
{
	return NULL;
}

void *vm_alloc(struct ksim_context *ctx, unsigned int size)
{
	return NULL;
}

void vm_free(struct ksim_context *ctx, void *addr)
{
	
}
