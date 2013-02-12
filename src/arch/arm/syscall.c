/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * arch/arm/syscall.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <stdio.h>

typedef int (*syscall_fn)(struct ksim_context *ctx, int arg0, int arg1, int arg2, int arg3);
static syscall_fn syscall_table[512];

#define SYSCALL_TABLE_SIZE (sizeof(syscall_table) / sizeof(syscall_table[0]))

int arm_init_syscalls()
{
	int i;
	
	for (i = 0; i < SYSCALL_TABLE_SIZE; i++)
		syscall_table[i] = (syscall_fn)syscall_not_impl;
	
	syscall_table[5] = (syscall_fn)syscall_open;
	syscall_table[6] = (syscall_fn)syscall_close;
	
	syscall_table[1] = (syscall_fn)syscall_exit;
	syscall_table[248] = (syscall_fn)syscall_exit_group;
	
	return 0;
}

int arm_perform_syscall(struct ksim_context *ctx, int syscall, int arg0, int arg1, int arg2, int arg3)
{
	if (syscall >= SYSCALL_TABLE_SIZE) {
		printf("error: syscall %d (0x%x) out of range\n", syscall, syscall);
		return -1;
	}
	
	return syscall_table[syscall](ctx, arg0, arg1, arg2, arg3);
}
