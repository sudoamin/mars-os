gdt_init:
      ; load GDT (reloading)
      lgdt [GDT_PTR]

      push 8
      push main ; main entry
      db 0x48
      retf


GDT:
      dq 0
      dq 0x0020980000000000

GDT_LEN: equ $-GDT

GDT_PTR:
      dw GDT_LEN - 1
      dq GDT
