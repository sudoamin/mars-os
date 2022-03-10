// https://wiki.osdev.org/Interrupt_Descriptor_Table

#ifndef IDT_H
#define IDT_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define TOTAL_INTERRUPTS 512

#include <stdint.h>

struct idt_desc {
  uint16_t offset_1;  // Offset bits 0 - 15
  uint16_t selector;  // Selector thats in our GDT
  uint8_t zero;       // Does nothing, unused set to zero
  uint8_t type_attr;  // Descriptor type and attributes
  uint16_t offset_2;  // Offset bits 16-31
} __attribute__((packed));

struct idtr_desc {
  uint16_t limit;  // Size of descriptor table -1
  uint32_t base;  // Base address of the start of the interrupt descriptor table
} __attribute__((packed));

void idt_init();

#endif