#ifndef __KSIM_MEM_H__
#define __KSIM_MEM_H__

#define GUEST_PAGE_SIZE 4096

struct vm_alloc_region {
	unsigned long base;
	unsigned int size;
	struct vm_alloc_region *next;
};

struct ksim_vm_info {
	unsigned long heap_base;
	unsigned long stack_bottom;
	struct vm_alloc_region *regions;
};

struct ksim_context;

extern struct ksim_vm_info *vm_create_info(struct ksim_context *ctx);
extern int vm_alloc_fixed(struct ksim_context *ctx, void __guest *addr, unsigned int size);
extern int vm_copy_to(struct ksim_context *ctx, void __guest *addr, void *src, unsigned int size);
extern int vm_copy_from(struct ksim_context *ctx, void __guest *addr, void *dest, unsigned int size);

extern char *vm_read_string(struct ksim_context *ctx, const char __guest *addr, unsigned int size);
extern void vm_free_string(struct ksim_context *ctx, char *str);
#endif
