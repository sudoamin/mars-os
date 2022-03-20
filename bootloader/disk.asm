; using the BIOS functions
; loads kernel to 0x9000
LoadKernel:
      mov ah, 2 ; Read sector command
      mov al, 100 ; x sector to read
      mov ch, 0 ; Cylinder low eight bits
      mov cl, 2 ; Read sector x
      mov dh, 0 ; head number
      mov bx, 0x9000 ; bootloader.asm
      int 0x13
      jc ErrLoadKernel
      
      mov si, MSGKernelLoaded
      call Print

      ret

ErrLoadKernel:
      mov si, MSGFailedLoadKernel
      call Print
      jmp End ; bootloader.asm

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
MSGFailedLoadKernel: db "* Failed to load kernel", 0ah, 0dh, 0
MSGKernelLoaded: db "Kernel has loaded", 0ah, 0dh, 0