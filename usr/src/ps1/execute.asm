section .text

global execute
extern main

execute:
      call main
      jmp $ ; why it do not work?