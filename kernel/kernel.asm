[BITS 64]
[ORG 0x9000]

init:
      ; IDT
      mov rdi, IDT
      mov rax, handler0 ; holds offset of the handler
      ; the offset is divided into three parts in the IDT entry
      mov [rdi], ax ; copy the lower 16 bits of the offset to the location that rdi points to
      shr rax, 16 ; shift offset in rax rightward by 16 bits. then the bits 16 to 31 are in ax
      mov [rdi+6], ax ; copy the bits 16 to 31 of the offset to the second part, which is at the 7th byte in the entry
      shr rax, 16 ; now bits 32 to 63 are in eax
      mov [rdi+8], eax ; copy the value in eax to the third part of the offset

      ; ; load GDT (reloading)
      lgdt [GDT_PTR]
      ; ; load IDT
      lidt [IDT_PTR]


      push 8
      push main ; main entry
      db 0x48
      retf

main:
      ; xor rbx, rbx
      ; div rbx
      ; OR
      ; int 0h

      jmp end

end:
      hlt
      jmp end

handler0:
      mov byte[0xb8000], 'D'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels

; %include "kernel/gdt.asm"
GDT:
      dq 0
      dq 0x0020980000000000

GDT_LEN: equ $-GDT

GDT_PTR:
      dw GDT_LEN - 1
      dq GDT

; %include "kernel/idt.asm"
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