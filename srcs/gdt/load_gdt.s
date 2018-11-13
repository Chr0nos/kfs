global _setGdt

; |LIMIT|----BASE----|
gdtr DW 0 ; For limit storage
     DD 0 ; For base storage
 
; setGdt(GDT, sizeof(GDT))
_setGdt:
   MOV   EAX, [esp + 4]
   MOV   [gdtr + 2], EAX
   MOV   AX, [ESP + 8]
   MOV   [gdtr], AX
   LGDT  [gdtr]
   RET
