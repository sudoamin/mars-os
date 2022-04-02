section .text

global syscall_write

syscall_write:
      ; allocate 16-byte space on the stack for the arguments
      sub rsp, 16

      ; rax holds the index number of syscall function
      ; the index is 0 for write screen function
      xor eax, eax

      ; copy first and second arguments to the new allocated space
      ; take look at https://wiki.osdev.org/Calling_Conventions
      mov [rsp], rdi
      mov [rsp+8], rsi

      ; rdi holds the number of arguments
      mov rdi, 2

      ; rsi points to address of arguments
      mov rsi, rsp

      ; the vector number is 0x80
      int 0x80

      ; after we return from the kernel,
      ; the return value is saved in rax

      ; to restore the stack
      add rsp, 16
      
      ret