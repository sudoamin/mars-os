Print:
      pusha
      mov bx, 0
.loop:
      lodsb
      cmp al, 0
      je .done
      call PrintChar
      jmp .loop
.done:
      popa
      ret

PrintChar: 
      mov ah, 0eh
      int 0x10
      ret

ClearScreen:
      mov al, 02h
      mov ah, 00h
      int 10h
      ret
