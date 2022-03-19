[BITS 16]
[ORG 0x7c00]

jmp start

%include "bootloader/print.asm"
; %include "bootloader/longmode.asm"
%include "bootloader/disk.asm"
%include "bootloader/gdt.asm"
%include "bootloader/idt.asm"

start:
      cli ; clear interrupts
      xor ax, ax
      mov ds, ax
      mov es, ax
      mov ss, ax
      mov sp, 0x7c00
      sti ; enables interrupts

      call clear_screen
      mov si, Message
      call print

      ; call LoadKernel ; TODO

      ; ENABLE PROTECTED MODE
      cli
      lgdt [GDT32Ptr]
      lidt [IDT32Ptr] ; invalid address, I do not want deal with idt in protected mode

      mov eax, cr0
      or eax, 1
      mov cr0, eax

      jmp 8:PMEntry

end:
      hlt
      jmp $

Message: db "RealMode ...", 0ah, 0dh, 0

%include "bootloader/pm.asm"

times 510-($-$$) db 0
dw 0xaa55

buffer: ; for loading kernel