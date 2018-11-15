// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef _IRQS_H_
#define _IRQS_H_
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define IRQ_COUNT	16
#define IDT_ENTRIES	256

// probably missing info
struct reg_itr {
	unsigned short int_no;
} __packed;


/* This exists in 'start.asm', and is used to load our IDT */
extern void _idt_load(void);
extern void _remap_pic(void);
extern void _eoi(int);

typedef unsigned int	irq_t;
typedef int		(*irq_handler_t)(struct reg_itr *i);

extern irq_t irq0;
extern irq_t irq1;
extern irq_t irq2;
extern irq_t irq3;
extern irq_t irq4;
extern irq_t irq5;
extern irq_t irq6;
extern irq_t irq7;
extern irq_t irq8;
extern irq_t irq9;
extern irq_t irq10;
extern irq_t irq11;
extern irq_t irq12;
extern irq_t irq13;
extern irq_t irq14;
extern irq_t irq15;

void	irqs_init(void);

#endif
