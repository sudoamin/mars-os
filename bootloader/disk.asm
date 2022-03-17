; %include "bootloader/print.asm"

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
      jmp end ; bootloader

DriveID: db 0
MSGDiskExtChecking: db "Checking Disk Extention ...", 0ah, 0dh, 0
MSGDiskExtNotSupported: db "* Disk extension not supported", 0ah, 0dh, 0
MSGDiskExtSupported: db "Disk extension is supported", 0ah, 0dh, 0