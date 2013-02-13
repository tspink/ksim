/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * loader/loader.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>

extern int elf_load(struct ksim_context *ctx, const char *path);
extern int dos_load(struct ksim_context *ctx, const char *path);
extern int raw_load(struct ksim_context *ctx, const char *path);
extern int hex_load(struct ksim_context *ctx, const char *path);

int ksim_load_binary(struct ksim_context *ctx, const char *path, enum arcsim_binary_type type)
{
	switch(type) {
	case BINARY_ELF:
		return elf_load(ctx, path);
	case BINARY_DOS:
		return dos_load(ctx, path);
	case BINARY_RAW:
		return raw_load(ctx, path);
	case BINARY_HEX:
		return hex_load(ctx, path);
	}
	
	return -1;
}
