#ifndef __KSIM_H__
#define __KSIM_H__

#define DEBUG
#ifdef DEBUG
# include <stdio.h>
# define kdbg(a...) fprintf(stderr, "debug: " a)
#else
# define kdbg(a...)
#endif

enum arcsim_arch_type {
	ARCH_UNKNOWN	= 0,
	ARCH_ARM	= 1,
	ARCH_X86	= 2,
};

enum arcsim_binary_type {
	BINARY_ELF	= 0,
	BINARY_DOS	= 1,
	BINARY_RAW	= 2,
	BINARY_HEX	= 3,
};

struct arcsim_ctx {
	void *priv;
};

struct arcsim_syscall_ctx {
	struct arcsim_ctx arcsim_ctx;
	unsigned int syscall_nr;
};

struct arcsim_load_binary_ctx {
	struct arcsim_ctx arcsim_ctx;
	const char *path;
	enum arcsim_binary_type type;
};

struct ksim_context;

typedef int (*arcsim_syscall_cb)(struct arcsim_syscall_ctx *);
typedef int (*arcsim_load_binary_cb)(struct arcsim_load_binary_ctx *);
typedef int (*ksim_syscall_cb)(struct ksim_context *, unsigned int, int, int, int, int);

struct arcsim_callbacks {
	int (*vm_read)(void *dest, unsigned long addr, unsigned int size);
	int (*vm_write)(void *src, unsigned long addr, unsigned int size);
};

struct arcsim_kernel_options {
	void *priv;
	
	arcsim_syscall_cb syscall;
	arcsim_load_binary_cb load_binary;
	
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

extern int ksim_load_binary(struct ksim_context *ctx, const char *path, enum arcsim_binary_type type);

extern int syscall_not_impl(struct ksim_context *ctx);
extern int syscall_open(struct ksim_context *ctx, const char __guest *path, int mode);
extern int syscall_close(struct ksim_context *ctx, int fd);
extern int syscall_exit(struct ksim_context *ctx);
extern int syscall_exit_group(struct ksim_context *ctx);

extern int vfs_init(struct ksim_context *context);
extern void vfs_exit(struct ksim_context *context);
extern int thread_init(struct ksim_context *context);
extern void thread_exit(struct ksim_context *context);

#endif
