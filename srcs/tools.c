#include "kernel.h"

/*
** return the lenght of "x" in chars
*/

uint32_t	nblen(int x)
{
	uint32_t		len;

	len = 1;
	if (x < 0)
		len++;
	while ((x) && (x /= 10))
		len++;
	return len;
}

/*
** write "nb" into buf until "size" is filled up
*/

uint32_t	itobuf(const int nb, char *buf, const size_t size)
{
	bool		negative = false;
	size_t		n;
	size_t		len;
	uint32_t	x;

	if (!size)
		return 0;
	len = nblen(nb);
    if (len > size) {
        len = size;
    }
	n = len;
	buf[n--] = '\0';
	if (nb < 0) {
		negative = true;
		x = (uint32_t)-nb;
	} else
		x = (uint32_t)nb;
	if (!x)
		buf[n--] = '0';
	while (x)
	{
		buf[n] = '0' + (x % 10);
		x /= 10;
		n--;
	}
	if (negative)
		buf[0] = '-';
	return len;
}

size_t strlen(const char *str)
{
	size_t len;

	for (len = 0; str[len]; len++)
		;
	return len;
}

bool    isdigit(const char c)
{
	return ((c >= '0') && (c <= '9'));
}

bool    isprint(const char c)
{
	return ((c >= 040) && (c <= 0176));
}

void	*memset(void *dest, const int c, const size_t n)
{
	size_t		p;
	const size_t	blk = INTIFY(SHORTIFY((unsigned char)c));

	p = 0;
	while ((p < n) && ((n - p) % CPU_BYTES_MAX))
		((unsigned char *)dest)[p++] = (unsigned char)c;
	while (p < n) {
		*(size_t *)((size_t)dest + p) = blk;
		p += CPU_BYTES_MAX;
	}
	return dest;
}

void	*memcpy(void *dest, const void *src, const size_t size)
{
	size_t		p;

	for (p = 0; (p < size) && (size - p) % CPU_BYTES_MAX; p++)
		((unsigned char *)dest)[p] = ((unsigned char *)src)[p];
	for (;p < size; p += CPU_BYTES_MAX)
		*(size_t *)((size_t)dest + p) = *(size_t *)((size_t)src + p);
	return dest;
}
