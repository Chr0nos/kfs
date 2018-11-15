// SPDX-License-Identifier: GPL-2.0-or-later
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

/*
 * Packed prevent compiler from optimization
 */

struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __packed;

void _setGdt(uint8_t *gdtr, size_t size);
// void reload_CS(void);
void _reloadSegments(void);
void init_gdt(void);

#endif
