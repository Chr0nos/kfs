CCASM=/opt/cross/i686/bin/i686-elf-as
CC=/opt/cross/i686/bin/i686-elf-gcc
CFLAGS=-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror -Wpedantic -Wsign-compare -Wpadded -Wshadow=global -Wvla -Wstrict-prototypes
LOOPDEV=/dev/loop2

SRCS_DIR=./srcs/
SRCS_FILES=kernel.c #irqs/io.c
SRCS=$(addprefix $(SRCS_DIR), $(SRCS_FILES))
ASM_SRCS=boot.s

OBJS=$(ASM_SRCS:%.s=%.o) $(SRCS:%.c=%.o)

all: kernel.bin

list:
	@echo $(OBJS)

boot.o: srcs/boot.s
	$(CCASM) $< -o boot.o

%.o: srcs/%.c
	$(CC) $(CFLAGS) -c $<

kernel.bin: $(OBJS)
	$(CC) -T linker.ld -o kernel.bin $(OBJS) -lgcc -nostdlib

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) boot kfs.img kfs.iso

kfs.iso: kernel.bin
	cp -v kernel.bin root/boot/kernel.bin
	grub-mkrescue --install-modules "multiboot legacycfg part_msdos part_acorn part_amiga part_apple part_bsd part_dvh part_gpt part_sun part_sunpc part_bsd part_dfly part_plan" -o kfs.iso root/ --compress=xz

kfs.img: kernel.bin
	cp -v kernel.bin root/boot/kernel.bin
	udisksctl unmount -b $(LOOPDEV) | true
	sudo losetup -d $(LOOPDEV) | true
	touch kfs.img
	truncate -s 8M kfs.img
	parted --script kfs.img mklabel msdos
	parted --script --align=opt kfs.img -a opt mkpart primary ext4 100s 100%
	sudo losetup $(LOOPDEV) -P kfs.img
	sudo mkfs.ext4 -F -L KFS $(LOOPDEV)p1
	sleep 1
	udisksctl mount -b $(LOOPDEV)p1
	sudo cp -rv root/* /run/media/$(shell whoami)/KFS/
	sudo grub-install --recheck --target=i386-pc --compress=xz $(LOOPDEV) --boot-directory=/run/media/$(shell whoami)/KFS/boot/ --install-modules "multiboot legacycfg part_msdos"
	udisksctl unmount -b $(LOOPDEV)p1
	sudo losetup -d $(LOOPDEV)

start: kfs.iso
	qemu-system-x86_64 kfs.iso

re: fclean all
