CCASM=i686-elf-as
CC=i686-elf-gcc

all:
	echo nope

boot:
	$(CCASM) boot.s -o boot

kernel:
	$(CC) kernel.c -o kernel

fclean:
	$(RM) boot
