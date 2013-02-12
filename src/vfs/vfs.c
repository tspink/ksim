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

int vfs_init_fd_table(struct vfs_fildes *table, int size)
{
	if (size < 3)
		return -1;
	
	/* Initialise stdin, stdout & stderr */
	table[0].state = OPEN;
	table[0].real_fd = 0;
	table[1].state = OPEN;
	table[1].real_fd = 1;
	table[2].state = OPEN;
	table[2].real_fd = 2;
	
	return 0; 
}

char *vfs_translate_path(struct ksim_context *context, const char *path)
{
    return (char *)path;
}
