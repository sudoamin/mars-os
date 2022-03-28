; Global Descriptor Table
; https://wiki.osdev.org/Global_Descriptor_Table

gdt_init:
      ; load GDT (reloading)
      mov rax, GDT_PTR
      lgdt [rax]

      ret
      
GDT:
      dq 0
      dq 0x0020980000000000 ; ring 0, code segment
      dq 0x0020F80000000000 ; ring 3, code segment, DPL=11=3
      ; P=1 DPL=11 W=1      
      dq 0x0000F20000000000 ; ring 3, data segment
TSS_DESC:
      dw TSS_LEN ; first rwo bytes are the lower 16 bits of TSS limit
      dw 0 ; base address of TSS (assign the address in the code)
      db 0 ; the lower 24 bits of base address
      db 0x89 ; the attribute field: P=1, DPL=00, TYPE=01001 specifes this is 64-bit TSS
      db 0
      db 0
      dq 0

GDT_LEN: equ $-GDT

GDT_PTR:
      dw GDT_LEN - 1
      dq GDT
