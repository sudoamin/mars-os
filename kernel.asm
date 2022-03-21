[BITS 64]
[ORG 0x9000]

Init:
      lgdt [GDT64Ptr]

      push 8
      push Main
      db 0x49
      retf

Main:


End:
      hlt
      jmp End


GDT64:
      dq 0
      dq 0x0020980000000000

GDT64Len: equ $-GDT64

GDT64Ptr:
      dw GDT64Len - 1
      dq GDT64
