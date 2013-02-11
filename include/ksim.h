#ifndef __KSIM_H__
#define __KSIM_H__

typedef int (*syscall_cb)(int, int, int, int, int);

struct arcsim_kernel_cb {
	syscall_cb syscall;
};

struct arcsim_kernel {
	int (*init)(struct arcsim_kernel_cb *);
	void (*exit)(void);
};

extern int ksim_syscall(int call, int arg0, int arg1, int arg2, int arg3);

#endif
