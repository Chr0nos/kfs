
#include "kernel.h"
#include "kernel.h"
#include "term.h"

void kernel_main(void)
{
	term_init();

// 	init_gdt();

	term_putstr("Hello World!\n");
	term.color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	term_putstr("I'm a green line !\n");
	term_puts_rainbow("I'm a multicolor line ! it's something !\n");

	char test[30];
	itobuf(63481, test, 30);
	term_putstr(test);
}
