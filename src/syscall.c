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
