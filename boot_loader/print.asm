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
