; %include "bootloader/print.asm"

TestLongModeSupport:
      pusha

      mov si, MSGLongModeChecking
      call print

      mov eax, 0x80000000
      cpuid
      cmp eax, 0x80000001
      jb LongModeNotSuported

      mov eax, 0x80000001
      cpuid
      test edx, (1<<29)
      jz LongModeNotSuported
      test edx, (1<<26)
      jz LongModeNotSuported

      mov si, MSGLongModeSupported
      call print

      popa
      ret

LongModeNotSuported: 
      mov si, MSGLongModeNotSupported
      call print
      jmp end ; bootloader

MSGLongModeChecking: db "Checking Long Mode Support ...", 0ah, 0dh, 0
MSGLongModeNotSupported: db "Long Mode not supported" 0ah, 0dh, 0
MSGLongModeSupported: db "Long Mode is supported", 0ah, 0dh, 0