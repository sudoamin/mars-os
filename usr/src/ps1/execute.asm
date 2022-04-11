section .text
global execute
extern main
; extern sys_exit

execute:
      call main
      ; call sys_exit
      jmp $ ; why it does not work?