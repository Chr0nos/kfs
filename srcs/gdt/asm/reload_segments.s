.section .text
.align=16
.global _reloadSegments

_reloadSegments:
   # Reload CS register containing code selector:
   jmp reload_CS # 0x08 points at the new code selector

reload_CS:
   # Reload data segment registers:
   mov 0x10, %AX # 0x10 points at the new data selector
   mov %AX, %DS
   mov %AX, %ES
   mov %AX, %FS
   mov %AX, %GS
   mov %AX, %SS
   ret
