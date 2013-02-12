/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * init.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>

static int kernel_init(struct arcsim_kernel_options *opt)
{
	switch (opt->arch_type) {
	case ARCH_ARM:
		opt->syscall = arm_perform_syscall;
		break;
	default:
		return -1;
	}

	return 0;
}

static void kernel_exit(void *priv)
{
}

const struct arcsim_kernel ksim_kernel = {
	.init = kernel_init,
	.exit = kernel_exit
};
