[BITS 64]
[ORG 0x9000]

Start:
      mov byte[0xb8000], 'K'
      mov byte[0xb8001], 0xa

      mov byte[0xb8002], 'R'
      mov byte[0xb8003], 0xa

End:
      hlt
      jmp End