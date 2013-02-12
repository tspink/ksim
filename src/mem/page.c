/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * mem/page.c
 * 
 * Coypright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-mem.h>
#include <malloc.h>

struct ksim_page *mem_map_guest_page(int page_index)
{
	struct ksim_page *page;
	page = malloc(sizeof(*page));
	if (!page)
		return NULL;
	
	page->page_index = page_index;
	page->guest_base = page_index * GUEST_PAGE_SIZE;
	page->host_base = 0;
	//page->host_base = mmap(NULL, GUEST_PAGE_SIZE, PROT_READ, MAP_PRIVATE)
	
	if (!page->host_base) {
		free(page);
		return NULL;
	}
	
	return page;
}

void mem_unmap_guest_page(struct ksim_page *page)
{
	free(page);
}
