[BITS 64]

extern kinit
extern kmain

global init

section .text

init:
      call disable_cursor
      ; mov bx, 90
      ; call set_cursor
      
      call gdt_init
      call pit_init
      call pic_init
      call tss_init

      push 8
      mov rax, main
      push rax
      db 0x48
      retf

main:
      call kinit
      call kmain

      sti ; enable interrupts

end:
      hlt
      jmp end

%include "kernel/int/pit.asm"
%include "kernel/gdt/gdt.asm"
%include "kernel/int/pic.asm"
%include "kernel/usermode.asm"
%include "kernel/tss.asm"
%include "console/cursor.asm"