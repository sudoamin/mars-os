[BITS 64]
[ORG 0x9000]

jmp init

%include "kernel/gdt/gdt.asm"
%include "kernel/idt/idt.asm"
%include "kernel/idt/pic.asm"
%include "kernel/idt/pit.asm"
%include "kernel/idt/handlers.asm"

init:
      call idt_init
      call pit_init
      call pic_init
      call gdt_init ; jumps to main

main:
      sti ; enable interrupts

      ; xor rbx, rbx
      ; div rbx
      ; OR
      ; int 0h

      jmp end

end:
      hlt
      jmp end
