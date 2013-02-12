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

struct ksim_arch {
    int (*init)();
    void (*exit)();
    syscall_cb syscall;
};

struct ksim_context {
    const struct ksim_arch *arch;
};

extern const struct ksim_arch arm_arch;

extern int syscall_not_impl(struct arcsim_syscall_ctx *ctx);

#endif
