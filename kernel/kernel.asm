[BITS 64]
[ORG 0x9000]

jmp init

%include "kernel/gdt.asm"
%include "kernel/idt.asm"
%include "kernel/pic.asm"
%include "kernel/pit.asm"

init:
      ; IDT
      mov rdi, IDT

      ; IDT entry for interrupt 0
      mov rax, handler0 ; holds offset of the handler
      ; the offset is divided into three parts in the IDT entry
      mov [rdi], ax ; copy the lower 16 bits of the offset to the location that rdi points to
      shr rax, 16 ; shift offset in rax rightward by 16 bits. then the bits 16 to 31 are in ax
      mov [rdi+6], ax ; copy the bits 16 to 31 of the offset to the second part, which is at the 7th byte in the entry
      shr rax, 16 ; now bits 32 to 63 are in eax
      mov [rdi+8], eax ; copy the value in eax to the third part of the offset

      ; IDT entry for the timer
      ; vector number of the timer is set to 32 in PIC
      ; so the address of entry is base of IDT + 32 * 16
      ; Note: each entry in 64-bit takes up 16-byte space
      mov rax, timer ; holds offset of the timer
      add rdi, 32*16 ; to make it point to the timer entry
      mov [rdi], ax
      shr rax, 16
      mov [rdi+6], ax
      shr rax, 16
      mov [rdi+8], eax


      lidt [IDT_PTR] ; load IDT

      ; load GDT (reloading)
      lgdt [GDT_PTR]

      push 8
      push main ; main entry
      db 0x48
      retf

main:
      ; xor rbx, rbx
      ; div rbx
      ; OR
      ; int 0h

      call pit_init
      call pic_init

      sti ; enable interrupts

      jmp end

end:
      hlt
      jmp end

handler0:
      mov byte[0xb8000], 'D'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels


timer:
      mov byte[0xb8000], 'T'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels
