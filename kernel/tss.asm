; Task State Segment structure
; https://wiki.osdev.org/Task_State_Segment

; the TSS is stored in the GDT

tss_init:
      mov rax, TSS
      mov rdi, TSS_DESC
      ; the lower 16 bits of address is in the third bytes of the TSS descriptor. 0-15
      mov [rdi+2], ax
      shr rax, 16 ; the bit 16 to 23 of the address is in al. 16-23
      mov [rdi+4], al
      shr rax, 8 ; 24-31
      mov [rdi+7], al
      shr rax, 8
      mov [rdi+8], eax ; 32-63
      
      mov ax, 0x20 ; the descriptor is the 5 entry in the GDT
      ltr ax

      ret

TSS:
      ; the first four bytes are reserved
      dd 0
      ; RSP0
      dq 0xffff800000008000
      ; other fields are not used in MarsOS
      times 88 db 0
      ; the address of IO permission
      ; assign size of TSS to it, which means IO permission is not used
      dd TSS_LEN

TSS_LEN: equ $-TSS