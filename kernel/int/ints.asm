; the interrupt handlers

handle_0:
      mov byte[0xb8000], 'D'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels


handle_timer:
      mov byte[0xb8000], 'T'
      ; inc byte[0xb8000]
      mov byte[0xb8001], 0xc

      ; when handling the hardware interrupts,
      ; we need to acknowledge before return from the handler
      ; otherwise we'll not receive the interrupt again
      mov al, 0x20
      out 0x20, al

      ; jmp end

      iretq ; can return to different privilege levels

handle_sirq:
      ; check to know this is a spurious interrupt
      ; read the ISR register of the PIC and check bit 7
      mov al, 11 ; write value 3 to command register (means reading ISR register)
      out 0x20, al
      in al, 0x20 ; read the command register
      test al, (1<<7)
      jz .end

      mov al, 0x20
      out 0x20, al

.end
      iretq