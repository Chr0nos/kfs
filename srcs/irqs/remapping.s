# reinitialize the PIC controllers, giving them specified vector offsets
#   rather than 8h and 70h, as configured by default 
 
.equ PIC1,		0x20		/* IO base address for master PIC */
.equ PIC2,		0xA0		/* IO base address for slave PIC */
.equ PIC1_COMMAND,	PIC1
.equ PIC1_DATA,		PIC1 + 1
.equ PIC2_COMMAND,	PIC2
.equ PIC2_DATA,		PIC2 + 1
.equ ICW1_ICW4,		0x01		# ICW4 (not) needed 
.equ ICW1_SINGLE,	0x02		# Single (cascade) mode 
.equ ICW1_INTERVAL4,	0x04		# Call address interval 4 (8) 
.equ ICW1_LEVEL,	0x08		# Level triggered (edge) mode 
.equ ICW1_INIT,		0x11		# Initialization - required!  (INIT 0x10 | ICW1_ICW4)
 
.equ ICW4_8086,		0x01		# 8086/88 (MCS-80/85) mode 
.equ ICW4_AUTO,		0x02		# Auto (normal) EOI 
.equ ICW4_BUF_SLAVE,	0x08		# Buffered mode/slave 
.equ ICW4_BUF_MASTER,	0x0C		# Buffered mode/master 
.equ ICW4_SFNM,		0x10		# Special fully nested (not) 
.equ OFFSET_1,		0x20		# offset1 - vector offset for master PIC, vectors on the master become offset1..offset1+7
.equ OFFSET_2,		0x28		# offset2 - same for slave PIC: offset2..offset2+7

 
	#inb(PIC1_DATA);                        # save masks
	#inb(PIC2_DATA);
.text
.global _remap_pic

_wait_io:
    mov 0, %al
    mov 0x80, %dx
	outb %al, %dx
    ret

_remap_pic:
    mov $PIC1_COMMAND, %al
    mov $ICW1_INIT, %dx
	outb %al, %dx			# starts the initialization sequence (in cascade mode)
    call _wait_io

    mov $PIC1_DATA, %al
    mov $OFFSET_1, %dx
	outb %al, %dx			# ICW2: Master PIC vector offset
    call _wait_io

    mov $PIC2_DATA, %al
	mov $OFFSET_2, %dx
    outb %al, %dx			# ICW2: Slave PIC vector offset
    call _wait_io

    mov $PIC2_DATA, %al
	mov 4, %dx
	outb %al, %dx				# ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    call _wait_io

    mov $PIC2_DATA, %al
	mov 2, %dx
	outb %al, %dx				# ICW3: tell Slave PIC its cascade identity (0000 0010)
    call _wait_io
 
    mov $PIC1_DATA, %al
    mov $ICW4_8086, %dx
    outb %al, %dx
    call _wait_io

    mov $PIC2_DATA, %al
    mov $ICW4_8086, %dx
    outb %al, %dx
    call _wait_io

    # restore saved masks.
    mov $PIC1_DATA, %al
    mov 0, %dx
    outb %al, %dx
    call _wait_io

    mov $PIC2_DATA, %al
    mov 0, %dx
    outb %al, %dx
    call _wait_io

    ret
