.section .text
.align=16
.global _setGdt

# |LIMIT|----BASE----|
gdtr:
    .byte 0 # For limit storage
    .short 0 # For base storage

_setGdt:
   mov 4(%esp), %eax
   mov %eax, gdtr(,1)
   mov 8(%esp), %ax
   mov %ax, (gdtr)
   lgdt (gdtr)
   ret
