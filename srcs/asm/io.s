section .text align=16
	global _inb
	global _outb
	global _io_wait

; uint8_t inb(uint16_t port);
_inb:
	mov eax, 0
	inb dil, bp

; void outb(uint16_t port, uint8_t val);
_outb:
	mov eax, 0
	outb al, bp

; void io_wait(void);
_io_wait:
	mov eax, 0
	outb al, 0x80
 
