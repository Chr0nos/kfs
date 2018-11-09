#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#ifdef __linux__
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#ifndef __i386__
# error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#define	VGA_WIDTH		80
#define VGA_HEIGHT		25
#define VGA_MAX			VGA_WIDTH * VGA_HEIGHT

static struct terminal		term;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | ((uint16_t)color << 8);
}

static size_t strlen(const char *str)
{
	size_t len;

	for (len = 0; str[len]; len++)
		;
	return len;
}

static void		term_clear(const uint16_t color)
{
	size_t			i;

	for (i = 0; i < VGA_MAX; i++)
		term.buffer[i] = color;
}

static void		term_init(void)
{
	term = (struct terminal) {
		.row = 0,
		.column = 0,
		.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK),
		.buffer = (uint16_t *)0xB8000
	};
	term_clear(vga_entry(' ', term.color));
}

void term_putchar_at(const char c, const uint8_t color,
	const size_t x, const size_t y)
{
	term.buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

static void term_putchar(const char c)
{
	term_putchar_at(c, term.color, term.column, term.row);
	if (++term.column == VGA_WIDTH) {
		term.column = 0;
		if (++term.row == VGA_HEIGHT)
			term.row = 0;
	}
}

void term_write(const char *str, const size_t size)
{
	size_t		i;

	for (i = 0; i < size; i++)
		term_putchar(str[i]);
}

void term_putstr(const char *str)
{
	term_write(str, strlen(str));
}

void kernel_main(void)
{
	/* Initialize terminal interface */
	term_init();
 
	/* Newline support is left as an exercise. */
	term_putstr("Hello, kernel World!\n");
}
