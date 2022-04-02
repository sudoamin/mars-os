section .text

extern int_handler

global vector0
global vector1
global vector2
global vector3
global vector4
global vector5
global vector6
global vector7
global vector8
; global vector9
global vector10
global vector11
global vector12
global vector13
global vector14
; global vector15
global vector16
global vector17
global vector18
global vector19
global vector32
global vector39
global sysint

global eoi
global read_isr
global read_cr2

global load_idt
global pstart

interrupt:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; inc byte[0xb8010]
    ; mov byte[0xb8011], 0xe

    mov rdi, rsp
    call int_handler
    ; and interrupt_return executes

interrupt_return:
    pop	r15
    pop	r14
    pop	r13
    pop	r12
    pop	r11
    pop	r10
    pop	r9
    pop	r8
    pop	rbp
    pop	rdi
    pop	rsi
    pop	rdx
    pop	rcx
    pop	rbx
    pop	rax

    ; skip int number and error code
    add rsp, 16
    iretq

vector0:
    push 0
    push 0
    jmp interrupt

vector1:
    push 0
    push 1
    jmp interrupt

vector2:
    push 0
    push 2
    jmp interrupt

vector3:
    push 0
    push 3
    jmp interrupt

vector4:
    push 0
    push 4
    jmp interrupt

vector5:
    push 0
    push 5
    jmp interrupt

vector6:
    push 0
    push 6
    jmp interrupt

vector7:
    push 0
    push 7
    jmp interrupt

vector8:
    push 8
    jmp interrupt

vector10:
    push 10
    jmp interrupt

vector11:
    push 11
    jmp interrupt

vector12:
    push 12
    jmp interrupt

vector13:
    push 13
    jmp interrupt

vector14:
    push 14
    jmp interrupt

vector16:
    push 0
    push 16
    jmp interrupt

vector17:
    push 17
    jmp interrupt

vector18:
    push 0
    push 18
    jmp interrupt

vector19:
    push 0
    push 19
    jmp interrupt

; the timer interrupt handler
vector32:
    push 0
    push 32
    jmp interrupt

vector39:
    push 0
    push 39
    jmp interrupt

sysint:
    push 0
    push 0x80
    jmp interrupt

; when handling the hardware interrupts,
; we need to acknowledge before return from the handler
; otherwise we'll not receive the interrupt again
eoi:
    mov al, 0x20
    out 0x20, al
    ret

; check to know this is a spurious interrupt
; read the ISR register of the PIC and check bit 7
read_isr:
    mov al, 11 ; write value 3 to command register (means reading ISR register)
    out 0x20, al
    in al, 0x20 ; read the command register
    ; moved to C
    ; test al, (1<<7)
    ; jz .end
    ret

load_idt:
    lidt [rdi]
    ret

read_cr2:
    mov rax, cr2
    ret

pstart:
    mov rsp, rdi
    jmp interrupt_return