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

// Global Description Table Registers
static uint8_t gdtr[GDT_LENGTH] = {0};

static struct gdt_descriptor const GDT[GDT_LENGTH] = {
	(const struct gdt_descriptor) {
		.base = 0,
		.limit = 0,
		.type = 0
	},
	(const struct gdt_descriptor) {
		.base = 0,
		.limit = 0xffffffff,
		.type = 0x9a
	},
	(const struct gdt_descriptor) {
		.base = 0,
		.limit = 0xffffffff,
		.type = 0x92
	},
	(const struct gdt_descriptor) {
		.base = (uint32_t)&tss,
		.limit = 0x68,
		.type = 0x89
	}
};

void setGdt(uint8_t *gdtr, size_t size);
void reload_CS(void);
void reloadSegments(void);

#endif
