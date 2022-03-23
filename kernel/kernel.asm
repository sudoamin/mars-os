[BITS 64]
[ORG 0x9000]

jmp init

%include "kernel/gdt/gdt.asm"
%include "kernel/idt/idt.asm"
%include "kernel/idt/pic.asm"
%include "kernel/idt/pit.asm"
%include "kernel/idt/handlers.asm"
%include "kernel/usermode.asm"

init:
      call idt_init
      call pit_init
      call pic_init
      call gdt_init ; jumps to main

main:
      ; sti ; enable interrupts

      ; xor rbx, rbx
      ; div rbx
      ; OR
      ; int 0h

      ; jumping to Ring 3
      ; we have to store 5 8-byte data on the statck:
      ; ss selector. the DPL of the descriptor and the RPL of ss selector are set to 3
      push 0x18|3
      ; RSP. the stack pointer
      push 0x7c00
      ; Rflags. only bit 1 which is required
      push 0x2
      ; cs (code segement) selector = 10 (third descriptor), RPL=3
      push 0x10|3
      ; RIP, the return address
      push um_main
      iretq

      jmp end

end:
      hlt
      jmp end
