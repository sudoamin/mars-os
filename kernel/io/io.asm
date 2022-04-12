section .text

global inb

inb:
    ; port number
    mov rdx, rdi
    ; read data
    in al, dx
    ret