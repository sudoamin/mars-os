bits 32

global _start

extern init
extern main

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x00

KERNEL_STACK_SIZE equ 4096

section .text
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd - (MAGIC_NUMBER + FLAGS)

_start:
cli
mov esp, kernel_stack + KERNEL_STACK_SIZE
call init
call main
hlt

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE