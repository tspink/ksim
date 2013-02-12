/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * vfs/vfs-syscall.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-vfs.h>
#include <stdio.h>

int syscall_open(struct ksim_context *ctx, __guest const char *gpath, int mode)
{
	char *path = read_guest_string(ctx, gpath);
	char *translated_path;
	
	if (!path)
		return -1;
	
	translated_path = vfs_translate_path(ctx, path);
	printf("SYSCALL: OPEN: %s, REAL: %s\n", path, translated_path);
	
	free_guest_string(path);
	return -1;
}

int syscall_close(struct ksim_context *ctx, int fd)
{
	return -1;
}
