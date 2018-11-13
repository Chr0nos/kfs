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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define INTMAX 2147483647
#define INTMIN -2147483648

uint32_t	itobuf(const int nb, char *buf, const size_t size);
size_t		strlen(const char *str);
bool    	isprint(const char c);

#endif
