#include "gdt.h"

static inline void kerror(const char *str)
{
    (void)str;
}

/**
 * \param target A pointer to the 8-byte GDT entry
 * \param source An arbitrary structure describing the GDT entry
 */
void encodeGdtEntry(uint8_t *target, struct gdt_descriptor source)
{
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && ((source.limit & 0xfff) != 0xfff)) {
        kerror("You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;
 
    // Encode the base 
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // And... Type
    target[5] = source.type;
}

// Global Description Table Registers
static uint8_t gdtr[GDT_LENGTH] = {0};

void init_gdt(void)
{
	size_t  i;

    GDT[0].base = (uint32_t)&tss;
	for (i = 0; i < GDT_LENGTH; ++i)
        encodeGdtEntry(&gdtr[i], GDT[i]);
	_setGdt(gdtr, sizeof(gdtr));
	// reload_CS();
    _reloadSegments();
}
