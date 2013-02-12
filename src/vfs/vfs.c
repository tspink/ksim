/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * vfs/vfs.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-vfs.h>
#include <malloc.h>

int vfs_init(struct ksim_context *context)
{
	context->vfs = malloc(sizeof(struct ksim_vfs_context));
	if (!context->vfs)
		return -1;
	return 0;
}

void vfs_exit(struct ksim_context *context)
{
	free(context->vfs);
}
