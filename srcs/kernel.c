// SPDX-License-Identifier: GPL-2.0-or-later
#include "kernel.h"
#include "term.h"
#include "./gdt/gdt.h"
#include "irqs/irqs.h"

static void	print_nums(struct terminal *term)
{
	char		test[30];
	size_t		len = itobuf(42, test, 30);
	const int	digits[] = {INTMIN, 0, 42, INTMAX};
	size_t		i;

	term->row = 20;
	term->column = 20;
	term->color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	i = 0;
	for (i = 0; i < 4; i++) {
		len = itobuf(digits[i], test, 30);
		term_write(term, test, len);
		term_putstr(term, " -> ");
	}
	term_putstr(term, "done");
}

void kernel_main(void)
{
	struct terminal		term;

	irqs_init();

	term_init(&term);
	term_putstr(&term, "Hello World!\n");
	term.color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	term_putstr(&term, "I'm a green line !\n");
	term_puts_rainbow(&term, "I'm a multicolor line ! it's something !\n");
	print_nums(&term);
}
