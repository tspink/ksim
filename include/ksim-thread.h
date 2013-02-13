#ifndef __KSIM_THREAD_H__
#define __KSIM_THREAD_H__

#define MAX_FILDES	16

#include <ksim-vfs.h>

struct ksim_page;
struct ksim_context;
struct ksim_vm_info;

struct ksim_thread {
    unsigned int pid;
    
    struct ksim_vm_info *vm;
    struct vfs_fildes fd_table[MAX_FILDES];
    struct ksim_page *page_mapping;
};

struct ksim_thread_context {
    struct ksim_thread *main_thread;
};

extern struct ksim_thread *thread_current(struct ksim_context *ctx);

#endif