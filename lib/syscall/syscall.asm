section .text

global sys_print
global sys_kbrd_read
global sys_sleep
global sys_exit
global sys_wait

sys_print:
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

sys_kbrd_read:
    mov eax,4
    xor edi,edi
    
    int 0x80

    ret

sys_sleep:
    sub rsp,8
    mov eax,1

    mov [rsp],rdi
    mov rdi,1
    mov rsi,rsp

    int 0x80

    add rsp,8
    
    ret

    
sys_exit:
    mov eax,2
    mov rdi,0

    int 0x80

    ret

sys_wait:
    mov eax,3
    mov rdi,0

    int 0x80

    ret