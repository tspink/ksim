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
	unsigned int length;
	char *guest_str;
	char *host_str;
	
	if (!page)
		return NULL;
	
	guest_str = (char *)(page->host_base + GUEST_ADDR_TO_PAGE_OFFSET(addr));
	length = strlen(guest_str);

	host_str = malloc(length + 1);
	strncpy(host_str, guest_str, length);
	
	mem_unmap_guest_page(page);
	
	return host_str;
}

void free_guest_string(char *str)
{
	free(str);
}
