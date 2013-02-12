#ifndef __KSIM_H__
#define __KSIM_H__

struct arcsim_syscall_ctx {
	void *priv;
};

typedef int (*syscall_cb)(struct arcsim_syscall_ctx *, int, int, int, int, int);

enum arcsim_arch_type {
	ARCH_UNKNOWN	= 0,
	ARCH_ARM	= 1,
	ARCH_X86	= 2,
};

struct arcsim_kernel_options {
	void *priv;
	syscall_cb syscall;
	enum arcsim_arch_type arch_type;
};

struct arcsim_kernel {
	int (*init)(struct arcsim_kernel_options *);
	void (*exit)(void *);
};

extern int arm_perform_syscall(struct arcsim_syscall_ctx *ctx, int syscall, int arg0, int arg1, int arg2, int arg3);
extern int syscall_not_impl(struct arcsim_syscall_ctx *ctx);

#endif
