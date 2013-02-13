#ifndef __KSIM_MEM_H__
#define __KSIM_MEM_H__

#define GUEST_HEAP_BASE 0x40000000
#define GUEST_PAGE_SIZE 4096
#define GUEST_ADDR_TO_PAGE(addr) ((unsigned long)addr / GUEST_PAGE_SIZE)
#define GUEST_ADDR_TO_PAGE_OFFSET(addr) ((unsigned long)addr % GUEST_PAGE_SIZE)

struct ksim_page {
	unsigned int ref;
	unsigned long page_index;
	unsigned long guest_base;
	unsigned long host_base;
	
	struct ksim_context *ctx;
	struct ksim_page *next;
};

struct ksim_vm_info {
	int tag;
};

struct ksim_context;

extern struct ksim_page *mem_map_guest_page(struct ksim_context *ctx, unsigned long page_index);
extern void mem_unmap_guest_page(struct ksim_page *page);
extern void mem_unmap_guest_page_nr(struct ksim_context *ctx, unsigned long page_index);

extern struct ksim_vm_info *vm_create_info(struct ksim_context *ctx);

#endif
