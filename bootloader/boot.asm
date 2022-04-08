; boot sector only loads the loader into 0x7e00 
; and then jump to it

[BITS 16]
[ORG 0x7c00]

jmp init

%include "bootloader/print.asm"

; in real mode we can use the BIOS functions
; so we just load the loader to 0x7e00
init:
      xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
      ; we have almost 30 KiB free space below of 0x7c00
      ; so we can use it for the stack
      mov sp, 0x7c00

      call clear_screen
      call load_loader

      jmp 0x7e00

end:
      hlt
      jmp $

load_loader:
      mov ah, 2 ; Read sector command
      mov al, 8 ; x sector to read
      mov ch, 0 ; Cylinder low eight bits
      mov cl, 2 ; Read sector x
      mov dh, 0 ; head number
      mov bx, 0x7e00
      int 0x13
      jc err_load_loader
      
      ret

err_load_loader:
      mov si, MSG_FAILED_LOAD_LOADER
      call print
      jmp end

MSG_FAILED_LOAD_LOADER: db "* Failed to load loader", 0ah, 0dh, 0

times 510-($-$$) db 0
dw 0xaa55