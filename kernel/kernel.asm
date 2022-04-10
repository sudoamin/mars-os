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
      call tss_init
      call pit_init
      call pic_init

      push 8
      mov rax, main
      push rax
      db 0x48
      retf

main:
      mov rsp, 0xffff800000200000

      call kinit
      ; sti
      call kmain

end:
      hlt
      jmp end

%include "kernel/int/pit.asm"
%include "kernel/gdt/gdt.asm"
%include "kernel/int/pic.asm"
%include "kernel/proc/tss.asm"
%include "kernel/console/cursor.asm"