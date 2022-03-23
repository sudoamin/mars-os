; Task State Segment
; https://wiki.osdev.org/Task_State_Segment

tss_init:
      mov rax, TSS
      ; the lower 16 bits of address is in the third bytes of the TSS descriptor. 0-15
      mov [TSS_DESC+2], ax
      shr rax, 16 ; the bit 16 to 23 of the address is in al. 16-23
      mov [TSS_DESC+4], al
      shr rax, 8 ; 24-31
      mov [TSS_DESC+7], al
      shr rax, 8
      mov [TSS_DESC+8], eax ; 32-63
      
      mov ax, 0x20 ; the descriptor is the 5 entry in the GDT
      ltr ax

      ret

TSS:
      ; the first four bytes are reserved
      dd 0
      ; RSP0
      dq 0x150000
      ; other fields are not used in MarsOS
      times 88 db 0
      ; the address of IO permission
      ; assign size of TSS to it, which means IO permission is not used
      dd TSS_LEN

TSS_LEN: equ $-TSS