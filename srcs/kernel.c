
#include "kernel.h"
#include "kernel.h"
#include "term.h"

void kernel_main(void)
{
	static struct terminal		term;

	term_init();

// 	init_gdt();

	term_putstr("Hello World!\n");

	term_putstr(&term, "Hello World!\n");
	term.color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	term_putstr(&term, "I'm a green line !\n");
	term_puts_rainbow(&term, "I'm a multicolor line ! it's something !\n");

	char test[30];
	itobuf(63481, test, 30);
	term.row = 20;
	term.column = 40;
	term.color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	term_putstr(&term, test);
	term_putstr(&term, "Hi !");

}
