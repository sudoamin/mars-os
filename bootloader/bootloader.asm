[BITS 16]
[ORG 0x7c00]

jmp start

%include "bootloader/print.asm"
%include "bootloader/longmode.asm"
%include "bootloader/disk.asm"

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

      call LoadKernel

      jmp end

end:
      hlt
      jmp $

Message: db "RealMode ...", 0ah, 0dh, 0

times 510-($-$$) db 0
dw 0xaa55

buffer: ; for loading kernel