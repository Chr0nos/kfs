CCASM=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Weverything

all:
	echo nope

boot:
	$(CCASM) boot.s -o boot

kernel:
	$(CC) $(CFLAGS) kernel.c -o kernel

fclean:
	$(RM) boot
