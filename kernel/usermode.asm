um_main:
      ; if the value of cs is 3, it means that we are running in Ring 3
      mov ax, cs
      and al, 11b
      cmp al, 3
      jne um_end

      mov byte[0xb8010], 'u'
      mov byte[0xb8011], 0xe

um_end:
      ; we cannot execute halt instruction in Ring 3
      jmp um_end