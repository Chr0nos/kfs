# reinitialize the PIC controllers, giving them specified vector offsets
#   rather than 8h and 70h, as configured by default 
 
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

_remap_pic:
	outb $PIC1_COMMAND, $ICW1_INIT			# starts the initialization sequence (in cascade mode)
	outb $0, $0x80
	outb $PIC1_DATA, $OFFSET_1			# ICW2: Master PIC vector offset
	outb $0, $0x80
	outb $PIC2_DATA, $OFFSET_2			# ICW2: Slave PIC vector offset
	outb $0, $0x80
	outb $PIC1_DATA, $4				# ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb $0, $0x80 
	outb $PIC2_DATA, $2				# ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb $0, $0x80
 
	outb $PIC1_DATA, $ICW4_8086
	outb $0, $0x80
	outb $PIC2_DATA, $ICW4_8086
	outb $0, $0x80
 
	outb $PIC1_DATA, $0   # restore saved masks.
	outb $PIC2_DATA, $0
