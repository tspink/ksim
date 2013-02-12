/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * thread/thread.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-thread.h>
#include <malloc.h>
#include <string.h>

#include "ksim-vfs.h"

static unsigned int current_pid = 1;

static inline unsigned int alloc_pid()
{
	return current_pid++;
}

static struct ksim_thread *create_thread_descriptor(int pid)
{
	struct ksim_thread *thread;
	thread = malloc(sizeof(*thread));
	if (!thread)
		return NULL;
    
	/* Store the PID for the thread. */
	thread->pid = pid;
    
	/* Initialise the file-descriptor table. */
	memset(thread->fd_table, 0, sizeof(thread->fd_table));
    
	return thread;
}

static void destroy_thread_descriptor(struct ksim_thread *thread)
{
	free(thread);
}

int thread_init(struct ksim_context *ctx)
{
	int rc;
	
	ctx->thread = malloc(sizeof(struct ksim_thread_context));
	if (!ctx->thread)
		return -1;
	
	ctx->thread->main_thread = create_thread_descriptor(alloc_pid());
	if (!ctx->thread->main_thread) {
		free(ctx->thread);
		return -1;
	}
	
	rc = vfs_init_fd_table(ctx->thread->main_thread->fd_table, MAX_FILDES);
	if (rc) {
	    destroy_thread_descriptor(ctx->thread->main_thread);
	    free(ctx->thread);
	    return -1;
	}
	
	return 0;
}

void thread_exit(struct ksim_context *ctx)
{
	destroy_thread_descriptor(ctx->thread->main_thread);
	free(ctx->thread);
}

struct ksim_thread *thread_current(struct ksim_context *ctx)
{
	return ctx->thread->main_thread;
}

int syscall_exit(struct ksim_context *ctx)
{
    return 0;
}