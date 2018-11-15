#ifndef TERM_H
# define TERM_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# define VGA_WIDTH		80
# define VGA_HEIGHT		25
# define VGA_MAX		VGA_WIDTH * VGA_HEIGHT

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#pragma pack(push, 1)

struct terminal {
	size_t		row;
	size_t		column;
	uint8_t		color;
	uint16_t	*buffer;
};

#pragma pack(pop)

void	term_init(struct terminal *term);
void	    term_puts_rainbow(struct terminal *term, const char *str);
void        term_putstr(struct terminal *term, const char *str);
void        term_write(struct terminal *term, const char *str, const size_t size);
void	    term_clear(struct terminal *term, const uint16_t color);

uint8_t     vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t    vga_entry(unsigned char uc, uint8_t color);

#endif
