IDT:
      %rep 256
            dw 0
            dw 8 ; the code segment descriptor that we currently use
            db 0
            ; attributes field
            db 0x8E ; D=1 DPL=00 TYPE=01110 -> 10001110
            dw 0
            dd 0
            dd 0
      %endrep

IDT_LEN: equ $-IDT

IDT_PTR:
      dw IDT_LEN-1
      dq IDT