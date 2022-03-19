GDT32:
      dq 0 ; each entry is 8 bytes, dq to allocate 8 bytes space
Code32:
      dw 0xffff
      dw 0
      db 0
      db 0x9a
      db 0xcf
      db 0
Data32:
      dw 0xffff
      dw 0
      db 0
      db 0x92
      db 0xcf
      db 0

GDT32Len: equ $ - GDT32
GDT32Ptr:
      dw GDT32Len - 1
      dd GDT32
