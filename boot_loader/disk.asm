TestDiskExtension:
      mov [DriveID], dl
      mov ah, 0x41
      mov bx, 0x55aa
      int 0x13
      jc PrintNotSuported
      cmp bx, 0x55aa
      jne PrintNotSuported
      ret

PrintNotSuported: 
      mov si, MSGDiskExtNotSupported
      call print
      jmp end

DriveID: db 0
MSGDiskExtNotSupported: db "Disk extension not supported", 0