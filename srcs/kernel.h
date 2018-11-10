#ifndef KERNEL_H
# define KERNEL_H

/* Check if the compiler thinks you are targeting the wrong operating system. */
# ifdef __linux__
#  error "You are not using a cross-compiler, you will most certainly run into trouble"
# endif

/* This tutorial will only work for the 32-bit ix86 targets. */
# ifndef __i386__
#  error "This tutorial needs to be compiled with a ix86-elf compiler"
# endif

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

#endif