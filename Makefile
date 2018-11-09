CCASM=i686-elf-as

all:
	echo nope

boot:
	$(CCASM) boot.s -o boot

fclean:
	$(RM) boot
