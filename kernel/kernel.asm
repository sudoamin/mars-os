[BITS 64]

extern kmain
global init

section .text

init:
      call gdt_init
      call pit_init
      call pic_init
      call tss_init

      push 8
      push main ; main entry
      db 0x48
      retf

main:
      mov rsp, 0x9000
      call kmain

end:
      hlt
      jmp end

%include "kernel/int/pit.asm"
%include "kernel/gdt/gdt.asm"
%include "kernel/int/pic.asm"
%include "kernel/int/ints.asm"
%include "kernel/usermode.asm"
%include "kernel/tss.asm"