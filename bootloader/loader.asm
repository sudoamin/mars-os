[BITS 16]
[ORG 0x7e00]

; in real mode we can use the BIOS functions
init:
      xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
      ; we have almost 30 KiB free space below of 0x7c00
      mov sp, 0x7c00

      call clear_screen
      mov si, MSG_WELCOME
      call print
      
      call get_mem_info

      ; ENABLE PROTECTED MODE
      lgdt [GDT32_PTR]
      lidt [IDT32_PTR] ; invalid address, I do not want deal with idt in protected mode

      mov eax, cr0
      or eax, 1
      mov cr0, eax

      jmp 8:protected_mode

end:
      hlt
      jmp $

MSG_WELCOME: db "MarsOS is booting...", 0ah, 0dh, 0

%include "bootloader/print.asm"
%include "bootloader/mem.asm"
; 32-bit
%include "bootloader/pm.asm"

times 4096-($-$$) db 0