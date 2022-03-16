[BITS 16]
[ORG 0x7c00]

jmp start

%include "boot_loader/print.asm"
%include "boot_loader/disk.asm"

start:
      xor ax, ax
      mov ds, ax
      mov es, ax
      mov ss, ax
      mov sp, 0x7c00

      call clear_screen
      mov si, Message
      call print

      jmp end

end:
      hlt
      jmp $

Message: db "RealMode ...", 0

times 510-($-$$) db 0
dw 0xaa55