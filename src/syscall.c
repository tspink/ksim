/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * syscall.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-mem.h>
#include <malloc.h>
#include <string.h>
#include <sys/mman.h>

int syscall_not_impl(struct ksim_context *ctx)
{
	printf("error: syscall not implemented\n");
	return -1;
}

char *read_guest_string(struct ksim_context *ctx, __guest const char *addr)
{
	struct ksim_page *page = mem_map_guest_page(ctx, GUEST_ADDR_TO_PAGE(addr));
	if (!page)
		return NULL;
	
	return (char *)(page->host_base + GUEST_ADDR_TO_PAGE_OFFSET(addr));
}

void free_guest_string(__guest const char *addr)
{
	mem_unmap_guest_page_nr(GUEST_ADDR_TO_PAGE(addr));
}
