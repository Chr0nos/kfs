#include "irqs.h"
#include "../kernel.h"

/* Present Ring	   Always
 * P	   DPL	   Always 01110 (14)
 * 1	   00      01110 => 0x8E
 */ 

/*
 *         | Protected-mode kernel  |
 *100000   +------------------------+
 *         | I/O memory hole        |
 *0A0000   +------------------------+
 *         | Reserved for BIOS      | Leave as much as possible unused
 *         ~                        ~
 *         | Command line           | (Can also be below the X+10000 mark)
 *X+10000  +------------------------+
 *         | Stack/heap             | For use by the kernel real-mode code.
 *X+08000  +------------------------+
 *         | Kernel setup           | The kernel real-mode code.
 *         | Kernel boot sector     | The kernel legacy boot sector.
 *       X +------------------------+
 *         | Boot loader            | <- Boot sector entry point 0x7C00
 *001000   +------------------------+
 *         | Reserved for MBR/BIOS  |
 *000800   +------------------------+
 *         | Typically used by MBR  |
 *000600   +------------------------+
 *         | BIOS use only          |
 *000000   +------------------------+
 */

#define KERNEL_SETUP_ADDR 0x08

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// probably missing info
struct reg_itr
{
    unsigned short int_no;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();
static uint32_t interrupt_handlers[256];

/* We'll leave you to try and code this function: take the
 *  argument 'base' and split it up into a high and low 16-bits,
 *  storing them in idt[num].base_hi and base_lo. The rest of the
 *  fields that you must set in idt[num] are fairly self-
 *  explanatory when it comes to setup
*/
void idt_set_gate(unsigned char num, unsigned long base,
		  unsigned short sel, unsigned char flags)
{
	idt[num].base_lo = (base >> 8) & 0xFF;
	idt[num].base_hi = base & 0xFF;
	idt[num].flags = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}



static inline void register_interrupt_handler(uint8_t n, uint32_t *handler)
{
  interrupt_handlers[n] = handler;
}

void irq_handler(struct reg_itr regs)
{
	eoi(regs.int_no);
	if (interrupt_handlers[regs.int_no] != 0)
	{
		// (uint32_t*)handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

extern void (*irq15)(void);

void	init_irqs(void)
{
	idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	idtp.base = &idt;
	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	remap_pic();

	idt_set_gate(32, irq0, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(33, irq1, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(34, irq2, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(35, irq3, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(36, irq4, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(37, irq5, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(38, irq6, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(39, irq7, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(40, irq8, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(41, irq9, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(42, irq10, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(43, irq11, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(44, irq12, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(45, irq13, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(46, irq14, KERNEL_SETUP_ADDR, 0x8E);
	idt_set_gate(47, irq15, KERNEL_SETUP_ADDR, 0x8E);
	
	idt_load();
}
