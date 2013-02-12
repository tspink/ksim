/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * syscall.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <malloc.h>

int syscall_not_impl(struct ksim_context *ctx)
{
	printf("error: syscall not implemented\n");
	return -1;
}

char *read_guest_string(struct ksim_context *ctx, __guest const char *addr)
{
	return NULL;
}

void free_guest_string(char *str)
{
	free(str);
}
