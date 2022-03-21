[BITS 64]
[ORG 0x9000]

init:
      lgdt [GDT64_PTR]

      push 8
      push main
      db 0x49
      retf

main:


end:
      hlt
      jmp end


GDT64:
      dq 0
      dq 0x0020980000000000

GDT64_LEN: equ $-GDT64

GDT64_PTR:
      dw GDT64_LEN - 1
      dq GDT64
