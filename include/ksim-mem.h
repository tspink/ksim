#ifndef __KSIM_MEM_H__
#define __KSIM_MEM_H__

#define GUEST_PAGE_SIZE 4096
#define GUEST_ADDR_TO_PAGE(addr) ((unsigned long)addr / GUEST_PAGE_SIZE)
#define GUEST_ADDR_TO_PAGE_OFFSET(addr) ((unsigned long)addr % GUEST_PAGE_SIZE)

struct ksim_page {
	unsigned int ref;
	unsigned long page_index;
	unsigned long guest_base;
	unsigned long host_base;
	
	struct ksim_page *next;
};

struct ksim_context;

extern struct ksim_page *mem_map_guest_page(struct ksim_context *ctx, int page_index);
extern void mem_unmap_guest_page(struct ksim_page *page);

#endif
