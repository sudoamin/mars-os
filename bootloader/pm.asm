%include "bootloader/longmode.asm"

[BITS 32]

; in protected mode and long mode we can not use the BIOS functions
PMEntry:
      mov ax, 0x10
      mov ds, ax
      mov es, ax
      mov ss, ax
      mov esp, 0x7c00 ; the stack pointer in protected mode

      ; TODO, print protected mode

      ; enable the A20 line
      ; https://wiki.osdev.org/A20_Line
      in al, 0x92
      or al, 2
      out 0x92, al

      ; ENABLE LONG MODE

      ; The address (0x80000 - 0x90000) may be used for BIOS data
      ; We can use memory area from 0x70000 to 0x80000 instead

      ; finds a free memory area and intialize the paging structure   
      cld
      mov edi, 0x80000
      xor eax, eax
      mov ecx, 0x10000/4
      rep stosd

      mov dword[0x80000], 0x81007
      mov dword[0x81000], 10000111b

      lgdt [GDT64Ptr]

      ; bit 5 in cr4 register is called physical address extension
      ; set it to 1
      mov eax, cr4
      or eax, (1<<5)
      mov cr4, eax

      ; copy the address of the page structure
      mov eax, 0x80000
      mov cr3, eax

      mov ecx, 0xc0000080
      rdmsr
      or eax, (1<<8)
      wrmsr

      mov eax, cr0
      or eax, (1<<31)
      mov cr0, eax

      ; 8 -> since each entry is 8 bytes and the code segment selector is the second entry
      ; then the offset of long mode
      jmp 8:LMEntry

      ; jmp PBEnd

; PBEnd:
;       hlt
;       jmp PBEnd

GDT32:
      ; the first entry
      dq 0 ; each entry is 8 bytes, dq to allocate 8 bytes space
; the code segment entry
Code32:
      dw 0xffff
      dw 0
      db 0
      db 0x9a
      db 0xcf
      db 0
; the data segment entry
Data32:
      dw 0xffff
      dw 0
      db 0
      db 0x92
      db 0xcf
      db 0

GDT32Len: equ $ - GDT32

GDT32Ptr:
      dw GDT32Len - 1
      dd GDT32
