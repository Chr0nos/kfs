#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel.h"

static struct terminal		term;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | ((uint16_t)color << 8);
}

size_t strlen(const char *str)
{
	size_t len;

	for (len = 0; str[len]; len++)
		;
	return len;
}

static bool isprint(const char c)
{
	return ((c >= 040) && (c <= 0176));
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
	if (c == '\n') {
		term.row++;
		term.column = 0;
	}
	else {
		term_putchar_at(c, term.color, term.column, term.row);
		term.column++;
	}
	if (term.column >= VGA_WIDTH)
	{
		term.column = 0;
		term.row++;
	}
	if (term.row >= VGA_WIDTH)
		term.row = 0;
}

void	term_puts_rainbow(const char *str)
{
	size_t			si;
	uint8_t			color;
	const uint8_t	color_origin = term.color;

	color = VGA_COLOR_BLUE;
	for (si = 0; str[si]; si++) {
		if (isprint(str[si])) {
			color++;
			term.color = color;
			term_putchar(str[si]);
			if (color >= VGA_COLOR_WHITE)
				color = VGA_COLOR_BLUE;
		}
	}
	term.color = color_origin;
}

void term_write(const char *str, const size_t size)
{
	size_t		i;

	for (i = 0; i < size; i++)
		term_putchar(str[i]);
}

void term_putstr(const char *str)
{
	while (*str)
		term_putchar(*(str++));
}

void kernel_main(void)
{
	term_init();
	term_putstr("Hello World!\n");
	term.color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	term_putstr("I'm a green line !\n");
	term_puts_rainbow("I'm a multicolor line ! it's something !\n");
}
