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

struct ksim_vm_info *vm_create_info(struct ksim_context *ctx)
{
	struct ksim_vm_info *vmi;
	
	vmi = malloc(sizeof(*vmi));
	if (!vmi) {
		return NULL;
	}
	
	vmi->tag = 0;
	
	return vmi;
}

void __guest *vm_alloc_fixed(struct ksim_context *ctx, void __guest *addr, unsigned int size)
{
	struct ksim_vm_info *vmi = thread_current(ctx)->vm;
	
	if (size % GUEST_PAGE_SIZE)
		size += GUEST_PAGE_SIZE - (size % GUEST_PAGE_SIZE);
	
	// TODO: Check allocation tables
	
	return NULL;
}

void __guest *vm_alloc(struct ksim_context *ctx, unsigned int size)
{
	void *addr = (void *)GUEST_HEAP_BASE;
	
	/* TODO:  Handle OOM condition. */
	while (!vm_alloc_fixed(ctx, addr, size)) {
		addr = (unsigned long)addr + GUEST_PAGE_SIZE;
	}
	
	return addr;
}

void vm_free(struct ksim_context *ctx, void __guest *addr)
{
	
}
