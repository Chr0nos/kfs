
.macro IRQ arg1 arg2
.global irq\arg1
  irq\arg1:
    cli
    push 0
    push \arg2
    jmp irq_common_stub
.endm

IRQ   0,  32
IRQ   1,  33
IRQ   2,  34
IRQ   3,  35
IRQ   4,  36
IRQ   5,  37
IRQ   6,  38
IRQ   7,  39
IRQ   8,  40
IRQ   9,  41
IRQ   10, 42
IRQ   11, 43
IRQ   12, 44
IRQ   13, 45
IRQ   14, 46
IRQ   15, 47

irq_common_stub:
   pusha		# Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov %ds, %ax		# Lower 16-bits of eax = ds.
   push %eax		# save the data segment descriptor

   movl 0x10, %ax		# load the kernel data segment descriptor
   movl %ax, %ds
   movl %ax, %es
   movl %ax, %fs
   movl %ax, %gs

   call irq_handler

   pop %ebx		# reload the original data segment descriptor
   movl %bx, %ds
   movl %bx, %es
   movl %bx, %fs
   movl %bx, %gs

   popa			# Pops edi,esi,ebp...
   add 8(%esp)		# Cleans up the pushed error code and pushed ISR number
   sti
   iret			# pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
