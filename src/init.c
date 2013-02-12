/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * init.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <malloc.h>

static int syscall(struct arcsim_syscall_ctx *syscall_ctx)
{
	struct ksim_context *ctx = syscall_ctx->priv;
	return ctx->arch->syscall(ctx, syscall_ctx->syscall_nr, 0, 0, 0, 0);
}

static int kernel_init(struct arcsim_kernel_options *opt, struct arcsim_callbacks *callbacks)
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
	ctx->opt = opt;
	ctx->arcsim = callbacks;
	
	/* Start the various subsystems. */
	rc = thread_init(ctx);
	if (rc) {
		free(ctx);
		arch->exit();
	}
	
	rc = vfs_init(ctx);
	if (rc) {
		thread_exit(ctx);
		free(ctx);
		arch->exit();
		return rc;
	}
	
	/* Populate the arcsim options structure. */
	opt->priv = ctx;
	opt->syscall = syscall;

	return 0;
}

static void kernel_exit(void *priv)
{
	struct ksim_context *ctx = priv;
	
	/* Finalise the various subsystems. */
	vfs_exit(ctx);
	
	/* Shutdown the architecture. */
	ctx->arch->exit();
	
	/* Release the ksim context object. */
	free(priv);
}

const struct arcsim_kernel ksim_kernel = {
	.init = kernel_init,
	.exit = kernel_exit
};
