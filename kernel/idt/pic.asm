[BITS 64]

; The 8259 Programmable Interrupt Controller (PIC)

; Read OSDEV document for more details
; https://wiki.osdev.org/8259_PIC

; PIC has command regsiter and data register
; each chip has its own register

pic_init:
      ; Initialization Command Word 1
      ; 0=1 1=0 2=0 3=0 4=1 5=0 6=0 7=0
      ; bit 4 means that this is the initialization command word
      ; bit 0 means that we use the last initialization command word
      mov al, 0x11
      out 0x20, al ; address of the master chip is 20
      out 0xa0, al ; address of the slave chip is a0

      ; Initialization Command Word 2
      ; specifies the starting vector number of the first IRQ
      ; Note: processor has defined the first 32 vectors for its own use.
      ; so we define the vectors number from 32 to 255
      ; each chip has 8 IRQs and first vector number of the master is 32
      ; and the starting vector number for the slave is 40 
      ; 0=0 1=0 2=0 3=0 4=0 5=1 6=0 7=0
      mov al, 32 
      out 0x21, al ; address of the master chip is 21
      mov al, 40 
      out 0xa1, al ; address of the slave chip is 0xa1

      ; Initialization Command Word 3
      ; indicats which IRQs is used for connecting the two PIC chips
      ; the slave is attached to the master via IRQ 2, bit 2 is 1 -> 4
      mov al, 4
      out 0x21, al ; data register
      mov al, 2 ; indentification
      out 0xa1, al
      
      ; Initialization Command Word 4
      ; it's used for selecting mode
      ; 0=1 meaning that x86 is used
      ; 1=0 is automatic end of interrupt, we don't use it.
      ; 2,3=0 means buffered mode is not used
      ; 4=0 specify fully nested mode
      mov al, 1
      out 0x21, al
      out 0xa1, al

      ; TODO
      ; only the IRQ 0 of the master will fire interrupts
      mov al, 11111110b ; only IRQ 0 of master 
      out 0x21, al
      mov al, 11111111b ; turn off slave interrupts
      out 0xa1, al
      
      ret
