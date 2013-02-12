#ifndef __KSIM_H__
#define __KSIM_H__

struct arcsim_syscall_ctx {
	void *priv;
	unsigned int syscall_nr;
};

struct ksim_context;

typedef int (*arcsim_syscall_cb)(struct arcsim_syscall_ctx *);
typedef int (*ksim_syscall_cb)(struct ksim_context *, unsigned int, int, int, int, int);

enum arcsim_arch_type {
	ARCH_UNKNOWN	= 0,
	ARCH_ARM	= 1,
	ARCH_X86	= 2,
};

struct arcsim_callbacks {
	void *(*map_page)(unsigned long);
	void (*unmap_page)(unsigned long);
};

struct arcsim_kernel_options {
	void *priv;
	arcsim_syscall_cb syscall;
	enum arcsim_arch_type arch_type;
};

struct arcsim_kernel {
	int (*init)(struct arcsim_kernel_options *, struct arcsim_callbacks *);
	void (*exit)(void *);
};

struct ksim_arch {
	int (*init)();
	void (*exit)();
	ksim_syscall_cb syscall;
};

struct ksim_vfs_context;
struct ksim_thread_context;

struct ksim_context {
	const struct ksim_arch *arch;
	struct arcsim_kernel_options *opt;
	struct arcsim_callbacks *arcsim;
	struct ksim_vfs_context *vfs;
	struct ksim_thread_context *thread;
};

extern const struct ksim_arch arm_arch;

#define __guest

extern int syscall_not_impl(struct ksim_context *ctx);
extern int syscall_open(struct ksim_context *ctx, __guest const char *path, int mode);
extern int syscall_close(struct ksim_context *ctx, int fd);
extern int syscall_exit(struct ksim_context *ctx);
extern int syscall_exit_group(struct ksim_context *ctx);

extern int vfs_init(struct ksim_context *context);
extern void vfs_exit(struct ksim_context *context);
extern int thread_init(struct ksim_context *context);
extern void thread_exit(struct ksim_context *context);

extern char *read_guest_string(struct ksim_context *context, __guest const char *str);
extern void free_guest_string(struct ksim_context *context, __guest const char *str);

#endif
