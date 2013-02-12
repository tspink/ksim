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
#include <ksim-thread.h>
#include <malloc.h>

static inline int inc_ref(struct ksim_page *page)
{
	return ++page->ref;
}

static inline int dec_ref(struct ksim_page *page)
{
	return --page->ref;
}

static void add_page(struct ksim_page **mapping, struct ksim_page *page)
{
	struct ksim_page **next = mapping;
	
	while (*next) {
		next = &(*next)->next;
	}
	
	(*next) = page;
}

static void remove_page(struct ksim_page *page)
{
	struct ksim_page *curr = thread_current(page->ctx)->page_mapping;
	
	while (curr) {
		if (curr->next == page) {
			curr->next = page->next;
			break;
		}
		
		curr = curr->next;
	}
}

static struct ksim_page *lookup_page(struct ksim_page **mapping, unsigned long page_index)
{
	struct ksim_page *page = *mapping;
	
	while (page) {
		if (page->page_index == page_index)
			return page;
		page = page->next;
	}
	
	return NULL;
}

struct ksim_page *mem_map_guest_page(struct ksim_context *ctx, unsigned long page_index)
{
	struct ksim_page **mapping = &thread_current(ctx)->page_mapping;
	struct ksim_page *page;
	
	page = lookup_page(mapping, page_index);
	if (page) {
		inc_ref(page);
		return page;
	}
	
	page = malloc(sizeof(*page));
	if (!page)
		return NULL;
	
	page->ctx = ctx;
	page->page_index = page_index;
	page->guest_base = page_index * GUEST_PAGE_SIZE;
	page->host_base = 0;
	page->ref = 1;
	//page->host_base = mmap(NULL, GUEST_PAGE_SIZE, PROT_READ, MAP_PRIVATE)
	
	if (!page->host_base) {
		free(page);
		return NULL;
	}
	
	add_page(mapping, page);
	return page;
}

void mem_unmap_guest_page(struct ksim_page *page)
{
	if (dec_ref(page) > 0)
		return;
	
	remove_page(page);
	free(page);
}

void mem_unmap_guest_page_nr(struct ksim_context *ctx, unsigned long page_index)
{
	struct ksim_page **mapping = &thread_current(ctx)->page_mapping;
	struct ksim_page *page = lookup_page(mapping, page_index);
	if (page)
		mem_unmap_guest_page(page);
}
