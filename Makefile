CCASM=/opt/cross/i686/bin/i686-elf-as
CC=/opt/cross/i686/bin/i686-elf-gcc
CFLAGS=-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror -Wsign-compare -Wpadded -Wshadow=global -Wvla -Wstrict-prototypes
LOOPDEV=/dev/loop2

SRCS_DIR=./srcs/
SRCS_FILES=kernel.c \
			tools.c \
			term.c \
			gdt/init_gdt.c

SRCS=$(addprefix $(SRCS_DIR), $(SRCS_FILES))

ASM_SRCS=asm/boot.s \
		gdt/asm/load_gdt.s \
		gdt/asm/reload_segments.s

ASM_OBJS=$(ASM_SRCS:%.s=%.o)

BUILDDIR=./build

OBJS=$(addprefix $(BUILDDIR)/, $(ASM_OBJS) $(SRCS_FILES:%.c=%.o))
ASM_OBJS_FULL=$(addprefix $(BUILDDIR)/, $(ASM_OBJS))

all: kernel.bin

$(BUILDDIR):
	mkdir -pv $(BUILDDIR)/asm
	mkdir -pv $(BUILDDIR)/gdt/asm

objs:
	@echo $(OBJS)

debug: kernel.bin
	qemu-system-i386 -kernel kernel.bin --enable-kvm -s -S

$(ASM_OBJS_FULL): $(BUILDDIR)/%.o: $(SRCS_DIR)/%.s
	$(CCASM) $< -o $@

$(BUILDDIR)/%.o: srcs/%.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(BUILDDIR) $(OBJS)
	$(CC) -T linker.ld -o kernel.bin $(OBJS) -lgcc -nostdlib

clean:
	$(RM) -r $(BUILDDIR)

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

start: kernel.bin
	qemu-system-x86_64 -kernel kernel.bin

re: fclean all

.PHONY: all re fclean clean start debug objs