#ifndef __KSIM_THREAD_H__
#define __KSIM_THREAD_H__

struct ksim_thread {
    unsigned int pid;
};

struct ksim_thread_context {
    struct ksim_thread *main_thread;
};

#endif