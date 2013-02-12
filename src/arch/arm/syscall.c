/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * arch/arm/syscall.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>

typedef int (*syscall_fn)(void);

static syscall_fn syscall_table[] = {
    (syscall_fn)syscall_not_impl
};

#define SYSCALL_TABLE_SIZE (sizeof(syscall_table) / sizeof(syscall_table[0]))

int arm_perform_syscall(struct arcsim_syscall_ctx *ctx, int syscall, int arg0, int arg1, int arg2, int arg3)
{
	if (syscall >= SYSCALL_TABLE_SIZE)
		return -1;
	
	return syscall_table[syscall]();
}
