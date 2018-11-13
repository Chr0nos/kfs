
struct gdt_descriptor {
	uint32_t base;
	uint32_t limit;
	uint32_t type;
};

struct gdt_descriptor GDT[4];

GDT[0] = {
	.base=0,
	.limit=0,
	.type=0
};                     // Selector 0x00 cannot be used

GDT[1] = {
	.base=0,
	.limit=0xffffffff,
	.type=0x9A
};         // Selector 0x08 will be our code

GDT[2] = {
	.base=0,
	.limit=0xffffffff,
	.type=0x92
};         // Selector 0x10 will be our data

GDT[3] = {
	.base=&myTss,
	.limit=sizeof(myTss),
	.type=0x89
}; // You can use LTR(0x18)
