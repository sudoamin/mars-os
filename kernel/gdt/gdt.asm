gdt_init:
      ; load GDT (reloading)
      lgdt [GDT_PTR]

      push 8
      push main ; main entry
      db 0x48
      retf


GDT:
      dq 0
      dq 0x0020980000000000 ; ring 0, code segment
      dq 0x0020F80000000000 ; ring 3, code segment, DPL=11=3
      ; P=1 DPL=11 W=1      
      dq 0x0000F20000000000 ; ring 3, data segment

GDT_LEN: equ $-GDT

GDT_PTR:
      dw GDT_LEN - 1
      dq GDT
