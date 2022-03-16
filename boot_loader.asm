[BITS 16]
[ORG 0x7c00]

start:
      xor ax, ax
      mov ds, ax
      mov es, ax
      mov ss, ax
      mov sp, 0x7c00

      mov si, Message
      call print

      jmp $

print:
      mov bx, 0
.loop:
      lodsb
      cmp al, 0
      je .done
      call print_char
      jmp .loop
.done:
      ret

print_char: 
      mov ah, 0eh
      int 0x10
      ret

Message: db "RealMode ...", 0

times 510-($-$$) db 0
dw 0xaa55