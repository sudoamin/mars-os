GDT:
      dq 0
      dq 0x0020980000000000

GDT_LEN: equ $-GDT

GDT_PTR:
      dw GDT_LEN - 1
      dq GDT
