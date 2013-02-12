#ifndef __KSIM_VFS_H__
#define __KSIM_VFS_H__

struct ksim_vfs_context {
	int tag;
};

enum vfs_fildes_state {
    INVALID,
    OPEN
};

struct vfs_fildes {
    enum vfs_fildes_state state;
    int real_fd;
};

extern int vfs_init_fd_table(struct vfs_fildes *fd_table, int size);

#endif