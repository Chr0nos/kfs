// SPDX-License-Identifier: GPL-2.0-or-later
#include "irqs.h"
#include "../kernel.h"

/* Present Ring	   Always
 * P	   DPL	   Always 01110 (14)
 * 1	   00      01110 => 0x8E
 *
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

struct idt_entry {
	unsigned short base_lo;
	unsigned short sel;        /* Our kernel segment goes here! */
	unsigned char always0;     /* This will ALWAYS be set to 0! */
	unsigned char flags;       /* Set using the above table! */
	unsigned short base_hi;
} __packed;

struct idt_ptr {
	unsigned short limit;
	unsigned int base;
} __packed;


/* Declare an IDT of 256 entries. Although we will only use the
 *  first 32 entries, the rest exists as a bit
 *  of a trap. If any undefined IDT entry is hit, it normally
 *  will cause an "Unhandled Interrupt" exception. Any descriptor
 *  for which the 'presence' bit is cleared (0) will generate an
 *  "Unhandled Interrupt" exception
 */

struct idt_entry	idt[IDT_ENTRIES];
struct idt_ptr		idtp;
static irq_handler_t	interrupt_handlers[IDT_ENTRIES] = {0};

/* We'll leave you to try and code this function: take the
 *  argument 'base' and split it up into a high and low 16-bits,
 *  storing them in idt[num].base_hi and base_lo. The rest of the
 *  fields that you must set in idt[num] are fairly self-
 *  explanatory when it comes to setup
 */

static void idt_set_gate(unsigned char num, unsigned long base,
			 unsigned short sel, unsigned char flags)
{
	idt[num].base_lo = (base >> 8) & 0xFF;
	idt[num].base_hi = base & 0xFF;
	idt[num].flags = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

static inline void register_interrupt_handler(uint8_t irq, irq_handler_t handler)
{
	interrupt_handlers[irq] = handler;
}

#include "../term.h"
static int	test(struct reg_itr *interrupt)
{
	struct terminal		tm;

	tm.row = 20;
	tm.column = 20;
	term_init(&tm);
	term_putstr(&tm, "banana");
	(void)interrupt;
	return 0;
}

void	irq_handler(struct reg_itr regs)
{
	test(&regs);
	_eoi(regs.int_no);
	if (interrupt_handlers[regs.int_no] != 0)
		interrupt_handlers[regs.int_no](&regs);
}

void	irqs_init(void)
{
	size_t		i;
	size_t		port;
	const irq_t	tab[IRQ_COUNT] = {
		irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7,
		irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
	};

	idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	idtp.base = (uint32_t)&idt;
	_remap_pic();
	port = 32;
	for (i = 0; i < IRQ_COUNT; i++)
		idt_set_gate(port++, tab[i], KERNEL_SETUP_ADDR, 0x8E);
	register_interrupt_handler(33, test);
	_idt_load();
}
