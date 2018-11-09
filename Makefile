CCASM=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror

all:
	echo nope

boot.o:
	$(CCASM) boot.s -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(CC) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc


fclean:
	$(RM) boot
