; %include "bootloader/print.asm"

; TODO
LoadKernel:
      pusha

      mov ah, 2 ; Read sector command
      mov al, 1 ; One sector to read
      mov ch, 0 ; Cylinder low eight bits
      mov cl, 2 ; Read sector two
      mov dh, 0 ; head number
      mov bx, buffer ; bootloader.asm
      int 0x13
      jc ErrLoadKernel

      mov si, buffer
      call print

      mov si, MSGKernelLoaded
      call print

      popa
      ret

ErrLoadKernel:
      mov si, MSGFailedLoadKernel
      call print
      jmp end ; bootloader.asm

TestDiskExtension:
      pusha

      mov si, MSGDiskExtChecking
      call print

      mov [DriveID], dl
      mov ah, 0x41
      mov bx, 0x55aa
      int 0x13
      jc DiskExtNotSuported
      cmp bx, 0x55aa
      jne DiskExtNotSuported

      mov si, MSGDiskExtSupported
      call print
      
      popa
      ret

DiskExtNotSuported: 
      mov si, MSGDiskExtNotSupported
      call print
      jmp end ; bootloader.asm

DriveID: db 0
MSGDiskExtChecking: db "Checking Disk Extention ...", 0ah, 0dh, 0
MSGDiskExtNotSupported: db "* Disk extension not supported", 0ah, 0dh, 0
MSGDiskExtSupported: db "Disk extension is supported", 0ah, 0dh, 0
MSGFailedLoadKernel: db "* Failed to load kernel", 0ah, 0dh, 0
MSGKernelLoaded: db "Kernel has loaded", 0ah, 0dh, 0