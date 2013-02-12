/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * syscall.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>

int syscall_not_impl(struct arcsim_syscall_ctx *ctx)
{
	return -1;
}
