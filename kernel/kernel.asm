[BITS 64]
[ORG 0x9000]

jmp init

%include "kernel/gdt/gdt.asm"
%include "kernel/idt/idt.asm"
%include "kernel/idt/pic.asm"
%include "kernel/idt/pit.asm"
%include "kernel/idt/handlers.asm"
%include "kernel/usermode.asm"
%include "kernel/idt/tss.asm"

init:
      call gdt_init
      call idt_init
      call pit_init
      call pic_init
      call tss_init

      push 8
      push main ; main entry
      db 0x48
      retf

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
      ; Rflags. bit 1 which is required and enable interrupts
      push 0x202
      ; cs (code segement) selector = 10 (third descriptor), RPL=3
      push 0x10|3
      ; RIP, the return address
      push um_main
      iretq

end:
      hlt
      jmp end
