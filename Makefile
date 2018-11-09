CCASM=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror

all:
	echo nope

boot.o:
	$(CCASM) boot.s -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel: boot.o kernel.o
	

fclean:
	$(RM) boot
