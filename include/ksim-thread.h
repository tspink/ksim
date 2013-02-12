#ifndef __KSIM_THREAD_H__
#define __KSIM_THREAD_H__

#define MAX_FILDES	16

#include <ksim-vfs.h>

struct ksim_thread {
    unsigned int pid;
    
    struct vfs_fildes fd_table[MAX_FILDES];
};

struct ksim_thread_context {
    struct ksim_thread *main_thread;
};

#endif