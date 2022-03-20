[BITS 64]

LMEntry:
      mov rsp, 0x7c00 ; the stack pointer in long mode or 64-bit mode

      ; TODO, print long mode

      jmp 0x9000


GDT64:
      dq 0
      ; If we do not want to jump to ring3, we do not need data segment for
      ; accessing data in memory

      ; the second entry:
      ; D=0  L=1  P=1  DPL=0  1  1  C=0
      ; 1 means that the descriptor is code segment descriptor
      ; DPL indicates privilege level of the segment (ring0)
      ; L or long mode should be 1, indicating that the code segment runs in 64-bit mode
      dq 0x0020980000000000

GDT64Len: equ $-GDT64

GDT64Ptr:
      dw GDT64Len - 1
      dd GDT64

; TestLongModeSupport:
;       pusha

;       mov si, MSGLongModeChecking
;       call print

;       mov eax, 0x80000000
;       cpuid
;       cmp eax, 0x80000001
;       jb LongModeNotSuported

;       mov eax, 0x80000001
;       cpuid
;       test edx, (1<<29)
;       jz LongModeNotSuported
;       test edx, (1<<26)
;       jz LongModeNotSuported

;       mov si, MSGLongModeSupported
;       call print

;       popa
;       ret

; LongModeNotSuported: 
;       mov si, MSGLongModeNotSupported
;       call print
;       jmp end ; bootloader

; MSGLongModeChecking: db "Checking Long Mode Support ...", 0ah, 0dh, 0
; MSGLongModeNotSupported: db "* Long Mode not supported" 0ah, 0dh, 0
; MSGLongModeSupported: db "Long Mode is supported", 0ah, 0dh, 0