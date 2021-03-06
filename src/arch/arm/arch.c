/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * arch/arm/arch.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>

extern int arm_perform_syscall(struct ksim_context *ctx, unsigned int syscall, int arg0, int arg1, int arg2, int arg3);
extern int arm_init_syscalls(void);

static int arm_init()
{
	return arm_init_syscalls();
}

static void arm_exit()
{
}

const struct ksim_arch arm_arch = {
    .init = arm_init,
    .exit = arm_exit,
    .syscall = arm_perform_syscall,
};
