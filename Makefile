CCASM=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror -Wpedantic -Wsign-compare -Wpadded -Wshadow=global -Wvla -Wstrict-prototypes

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

kfs.iso: kernel.bin
	cp -v kernel.bin root/boot/kernel.bin
	grub-mkrescue --install-modules "multiboot legacycfg part_msdos part_acorn part_amiga part_apple part_bsd part_dvh part_gpt part_sun part_sunpc part_bsd part_dfly part_plan" -o kfs.iso root/ --compress=xz

start: kfs.iso
	qemu-system-x86_64 kfs.iso

re: fclean all