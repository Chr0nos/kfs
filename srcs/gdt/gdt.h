#ifndef _GDT_H_
# define _GDT_H_
# include <stddef.h>
# include <stdint.h>
# define GDT_LENGTH 4

static const uint32_t tss;

struct gdt_descriptor {
	uint32_t base;
	uint32_t limit;
	uint32_t type;
};

static struct gdt_descriptor GDT[GDT_LENGTH] = {
	(struct gdt_descriptor) {
		.base = 0,
		.limit = 0,
		.type = 0
	},
	(struct gdt_descriptor) {
		.base = 0,
		.limit = 0xffffffff,
		.type = 0x9a
	},
	(struct gdt_descriptor) {
		.base = 0,
		.limit = 0xffffffff,
		.type = 0x92
	},
	(struct gdt_descriptor) {
		.base = 0,
		.limit = 0x68,
		.type = 0x89
	}
};

void _setGdt(uint8_t *gdtr, size_t size);
// void reload_CS(void);
void _reloadSegments(void);
void init_gdt(void);

#endif
