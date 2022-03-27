// https://wiki.osdev.org/Interrupt_Descriptor_Table

#ifndef IDT_H
#define IDT_H

#define KERNEL_CODE_SELECTOR 8
#define TOTAL_INTERRUPTS 256

#include <stdint.h>

struct idt_entry {
  uint16_t offset_1;    // Offset bits 0 - 15
  uint16_t selector;    // Selector thats in our GDT
  uint8_t reserved_1;   // Does nothing, unused set to zero
  uint8_t attr;         // Descriptor type and attributes
  uint16_t offset_2;    // Offset bits 16-31
  uint32_t offset_3;    // Offset bits 32-63
  uint32_t reserved_2;  // Does nothing, unused set to zero
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;  // Size of descriptor table -1
  uint64_t base;  // Base address of the start of the interrupt descriptor table
} __attribute__((packed));

void idt_init(void);

struct trap_frame {
  int64_t r15; // RSP  low address
  int64_t r14;
  int64_t r13;
  int64_t r12;
  int64_t r11;
  int64_t r10;
  int64_t r9;
  int64_t r8;
  int64_t rbp;
  int64_t rdi;
  int64_t rsi;
  int64_t rdx;
  int64_t rcx;
  int64_t rbx;
  int64_t rax;
  int64_t trapno;
  int64_t errorcode;
  int64_t rip;
  int64_t cs;
  int64_t rflags;
  int64_t rsp;
  int64_t ss; // high address
};

void vector0(void);
void vector1(void);
void vector2(void);
void vector3(void);
void vector4(void);
void vector5(void);
void vector6(void);
void vector7(void);
void vector8(void);
void vector10(void);
void vector11(void);
void vector12(void);
void vector13(void);
void vector14(void);
void vector16(void);
void vector17(void);
void vector19(void);
void vector32(void);
void vector39(void);
void eoi(void);
unsigned char read_isr(void);
extern void load_idt(struct idt_ptr *ptr);

#endif