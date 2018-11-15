.section .text
.align=16
.global _eoi

.equ PIC1,		0x20		/* IO base address for master PIC */
.equ PIC2,		0xA0		/* IO base address for slave PIC */
.equ PIC1_COMMAND,	PIC1
.equ PIC1_DATA,		PIC1 + 1
.equ PIC2_COMMAND,	PIC2
.equ PIC2_DATA,		PIC2 + 1



_eoi:
	add	4(%esp), %eax
	cmp	%eax, 0  
	jz	pic1
pic2:
    mov PIC2_COMMAND, %al
    mov PIC2, %dx
    outb %al, %dx
pic1:
    mov PIC1_COMMAND, %al
    mov PIC1, %dx
    outb %al, %dx
