handle_0:
      mov byte[0xb8000], 'D'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels


handle_timer:
      mov byte[0xb8000], 'T'
      mov byte[0xb8001], 0xc

      jmp end

      iretq ; can return to different privilege levels
