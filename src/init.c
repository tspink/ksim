/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * init.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <malloc.h>

static int kernel_init(struct arcsim_kernel_options *opt)
{
	struct ksim_context *ctx;
	const struct ksim_arch *arch;
	int rc;
	
	/* Determine which architecture we're going to be emulating. */
	switch (opt->arch_type) {
	case ARCH_ARM:
		arch = &arm_arch;
		break;
	default:
		return -1;
	}
	
	/* Initialise the architecture module. */
	rc = arch->init();
	if (rc) {
		return rc;
	}
	
	/* Allocate storage space for the ksim context object. */
	ctx = malloc(sizeof(*ctx));
	if (!ctx) {
		arch->exit();
		return -1;
	}
	
	/* Initialise the ksim context object. */
	ctx->arch = arch;
	
	/* Populate the arcsim options structure. */
	opt->priv = ctx;
	opt->syscall = arch->syscall;

	return 0;
}

static void kernel_exit(void *priv)
{
	/* Release the ksim context object. */
	free(priv);
}

const struct arcsim_kernel ksim_kernel = {
	.init = kernel_init,
	.exit = kernel_exit
};
