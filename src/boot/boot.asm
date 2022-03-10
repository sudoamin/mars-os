bits 32
global _start
extern init
extern main

section .text
    align 4
    dd 0x1BADB002 ; magic
    dd 0x00 ; flag
    dd - (0x1BADB002 + 0x00) ; checksum
_start:
cli
mov esp, stack
call init
call main
hlt

section .bss
resb 8192
stack: