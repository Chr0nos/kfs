CCASM=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror

all: kernel.bin

boot.o:
	$(CCASM) boot.s -o boot.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

kernel.bin: boot.o kernel.o
	$(CC) -T linker.ld -o kernel.bin boot.o kernel.o -lgcc -nostdlib

clean:
	$(RM) boot.o kernel.o

fclean: clean
	$(RM) boot

re: fclean