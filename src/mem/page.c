/*
 * ksim - Kernel Simulator for ArcSim
 * 
 * mem/page.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <ksim.h>
#include <ksim-mem.h>
#include <malloc.h>

static struct ksim_page *mapped_pages;

static inline int inc_ref(struct ksim_page *page)
{
	return ++page->ref;
}

static inline int dec_ref(struct ksim_page *page)
{
	return --page->ref;
}

static void add_page(struct ksim_page *page)
{
	struct ksim_page **next = &mapped_pages;
	
	while (*next) {
		next = &(*next)->next;
	}
	
	(*next) = page;
}

static struct ksim_page *lookup_page(int page_index)
{
	struct ksim_page *page = mapped_pages;
	
	while (page) {
		if (page->page_index == page_index)
			return page;
		page = page->next;
	}
	
	return NULL;
}

struct ksim_page *mem_map_guest_page(int page_index)
{
	struct ksim_page *page;
	
	page = lookup_page(page_index);
	if (page) {
		inc_ref(page);
		return page;
	}
	
	page = malloc(sizeof(*page));
	if (!page)
		return NULL;
		
	page->page_index = page_index;
	page->guest_base = page_index * GUEST_PAGE_SIZE;
	page->host_base = 0;
	page->ref = 1;
	//page->host_base = mmap(NULL, GUEST_PAGE_SIZE, PROT_READ, MAP_PRIVATE)
	
	if (!page->host_base) {
		free(page);
		return NULL;
	}
	
	add_page(page);
	return page;
}

void mem_unmap_guest_page(struct ksim_page *page)
{
	if (dec_ref(page) > 0)
		return;
	
	free(page);
}
