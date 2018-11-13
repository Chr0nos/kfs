#ifndef
#define _GDT_H_

struct gdt_descriptor {
	uint32_t base;
	uint32_t limit;
	uint32_t type;
};

uint8_t gdtr[4] = {0};
struct gdt_descriptor GDT[4];


static const uint32_t tss;
static const uint32_t tss_length = 0x68;

// FLAT SETUP
GDT[0] = {
	.base=0,
	.limit=0,
	.type=0
};                     // Selector 0x00 cannot be used

GDT[1] = {
	.base=0,
	.limit=0xffffffff,
	.type=0x9A
};         // Selector 0x08 will be our code, DS

GDT[2] = {
	.base=0,
	.limit=0xffffffff,
	.type=0x92
};         // Selector 0x10 will be our data, CS

GDT[3] = {
	.base=&tss,
	.limit=sizeof(tss),
	.type=0x89
}; // You can use LTR(0x18)


#endif
