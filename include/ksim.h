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

struct ksim_vfs_context;
struct ksim_thread_context;

struct ksim_context {
	const struct ksim_arch *arch;
	struct arcsim_kernel_options *opt;
	struct ksim_vfs_context *vfs;
	struct ksim_thread_context *thread;
};

extern const struct ksim_arch arm_arch;

extern int syscall_not_impl(struct arcsim_syscall_ctx *ctx);
extern int vfs_init(struct ksim_context *context);
extern void vfs_exit(struct ksim_context *context);
extern int thread_init(struct ksim_context *context);
extern void thread_exit(struct ksim_context *context);

#endif
