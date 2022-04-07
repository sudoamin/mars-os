; using the BIOS functions
; loads kernel to 0x9000
; 0x15800 - 0x9000 = 0xC800 = 512000 bytes = 50 KBs
load_kernel:
      ; mov si, MSG_LOADING_KERNEL
      ; call print

      mov ah, 2 ; Read sector command
      mov al, 100 ; x sector to read
      mov ch, 0 ; Cylinder low eight bits
      mov cl, 9 ; Read sector x
      mov dh, 0 ; head number
      mov bx, 0x9000
      int 0x13
      jc err_load_kernel
      
      ; mov si, MSG_KERNEL_LOADED
      ; call print

      ret

err_load_kernel:
      mov si, MSG_FAILED_LOAD_KERNEL
      call print
      jmp end ; bootloader.asm

; TestDiskExtension:
;       pusha

;       mov si, MSGDiskExtChecking
;       call print

;       mov [DriveID], dl
;       mov ah, 0x41
;       mov bx, 0x55aa
;       int 0x13
;       jc DiskExtNotSuported
;       cmp bx, 0x55aa
;       jne DiskExtNotSuported

;       mov si, MSGDiskExtSupported
;       call print
      
;       popa
;       ret

; DiskExtNotSuported: 
;       mov si, MSGDiskExtNotSupported
;       call print
;       jmp end ; bootloader.asm

; DriveID: db 0
; MSGDiskExtChecking: db "Checking Disk Extention ...", 0ah, 0dh, 0
; MSGDiskExtNotSupported: db "* Disk extension not supported", 0ah, 0dh, 0
; MSGDiskExtSupported: db "Disk extension is supported", 0ah, 0dh, 0
; MSG_LOADING_KERNEL: db "Loading kernel...", 0ah, 0dh, 0
MSG_FAILED_LOAD_KERNEL: db "* Failed to load kernel", 0ah, 0dh, 0
; MSG_KERNEL_LOADED: db "Kernel has loaded", 0ah, 0dh, 0