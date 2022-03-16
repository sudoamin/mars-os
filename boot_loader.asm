[BITS 16]
[ORG 0x7c00]

start:
      xor ax, ax
      mov ds, ax
      mov es, ax
      mov ss, ax
      mov sp, 0x7c00

      call clear_screen
      
      mov si, Message
      call print

      jmp $

print:
      pusha
      mov bx, 0
.loop:
      lodsb
      cmp al, 0
      je .done
      call print_char
      jmp .loop
.done:
      popa
      ret

print_char: 
      mov ah, 0eh
      int 0x10
      ret

clear_screen:
      mov al, 02h
      mov ah, 00h
      int 10h
      ret

Message: db "RealMode ...", 0

times 510-($-$$) db 0
dw 0xaa55