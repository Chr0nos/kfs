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

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load(void);

extern void (*irq0)(void);
extern void (*irq1)(void);
extern void (*irq2)(void);
extern void (*irq3)(void);
extern void (*irq4)(void);
extern void (*irq5)(void);
extern void (*irq6)(void);
extern void (*irq7)(void);
extern void (*irq8)(void);
extern void (*irq9)(void);
extern void (*irq10)(void);
extern void (*irq11)(void);
extern void (*irq12)(void);
extern void (*irq13)(void);
extern void (*irq14)(void);
extern void (*irq15)(void);

#endif
