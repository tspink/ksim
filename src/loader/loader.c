/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * loader/loader.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-binary.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int ksim_load_binary(struct ksim_context *ctx, const char *path, enum arcsim_binary_type type)
{
	struct ksim_binary *binary;
	struct stat st;
	int fd, rc;
	
	binary = malloc(sizeof(*binary));
	if (!binary)
		return -1;
	
	binary->path = path;
	binary->type = type;
	binary->ctx = ctx;
	
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		free(binary);
		return -1;
	}
	
	rc = fstat(fd, &st);
	if (rc) {
		close(fd);
		free(binary);
		return -1;
	}
	
	binary->size = st.st_size;
	binary->base = mmap(NULL, binary->size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	
	if (!binary->base) {
		free(binary);
		return -1;
	}
	
	switch(type) {
	case BINARY_ELF:
		rc = elf_load(binary);
		break;
	case BINARY_DOS:
		rc = dos_load(binary);
		break;
	case BINARY_RAW:
		rc = raw_load(binary);
		break;
	case BINARY_HEX:
		rc = hex_load(binary);
		break;
	default:
		rc = -1;
		break;
	}
	
	if (rc) {
		munmap(binary->base, binary->size);
		free(binary);
	}
	
	return rc;
}
