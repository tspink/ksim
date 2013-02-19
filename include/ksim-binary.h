#ifndef __KSIM_BINARY_H__
#define __KSIM_BINARY_H__

enum arcsim_binary_type;
struct ksim_context;

struct ksim_binary {
	struct ksim_context *ctx;
	const char *path;
	enum arcsim_binary_type type;
	
	void *priv;
	
	void *base;
	unsigned int size;
	
	unsigned long entry_point;
};

extern int elf_load(struct ksim_binary *binary);
extern int dos_load(struct ksim_binary *binary);
extern int raw_load(struct ksim_binary *binary);
extern int hex_load(struct ksim_binary *binary);

#endif