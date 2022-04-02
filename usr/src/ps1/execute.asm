section .text

global execute
extern main

execute:
      call main
      jmp $