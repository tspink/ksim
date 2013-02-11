#include <ksim.h>

static int kernel_init(struct arcsim_kernel_cb *cb)
{
	cb->syscall = ksim_syscall;
	return 0;
}

static void kernel_exit()
{
}

const struct arcsim_kernel ksim_kernel = {
	.init = kernel_init,
	.exit = kernel_exit
};
