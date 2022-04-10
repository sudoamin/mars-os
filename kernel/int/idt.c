#include <kernel/include/idt.h>

static struct idt_entry idt[TOTAL_INTERRUPTS];
static struct idt_ptr *idt_ptr;

static void idt_entry_set(struct idt_entry *entry, uint64_t addr,
                          uint8_t attr) {
  entry->offset_1 = (uint16_t)addr;
  entry->selector = KERNEL_CODE_SELECTOR;
  entry->attr = attr;
  entry->offset_2 = (uint16_t)(addr >> 16);
  entry->offset_3 = (uint32_t)(addr >> 32);
}

void init_idt() {
  idt_entry_set(&idt[0], (uint64_t)vector0, 0x8e);
  idt_entry_set(&idt[1], (uint64_t)vector1, 0x8e);
  idt_entry_set(&idt[2], (uint64_t)vector2, 0x8e);
  idt_entry_set(&idt[3], (uint64_t)vector3, 0x8e);
  idt_entry_set(&idt[4], (uint64_t)vector4, 0x8e);
  idt_entry_set(&idt[5], (uint64_t)vector5, 0x8e);
  idt_entry_set(&idt[6], (uint64_t)vector6, 0x8e);
  idt_entry_set(&idt[7], (uint64_t)vector7, 0x8e);
  idt_entry_set(&idt[8], (uint64_t)vector8, 0x8e);
  idt_entry_set(&idt[10], (uint64_t)vector10, 0x8e);
  idt_entry_set(&idt[11], (uint64_t)vector11, 0x8e);
  idt_entry_set(&idt[12], (uint64_t)vector12, 0x8e);
  idt_entry_set(&idt[13], (uint64_t)vector13, 0x8e);
  idt_entry_set(&idt[14], (uint64_t)vector14, 0x8e);
  idt_entry_set(&idt[16], (uint64_t)vector16, 0x8e);
  idt_entry_set(&idt[17], (uint64_t)vector17, 0x8e);
  idt_entry_set(&idt[19], (uint64_t)vector19, 0x8e);
  idt_entry_set(&idt[32], (uint64_t)vector32, 0x8e);
  idt_entry_set(&idt[33], (uint64_t)vector33, 0x8e);
  idt_entry_set(&idt[39], (uint64_t)vector39, 0x8e);

  // 0xee, set dpl to 3 instead of 0
  // we will fire the interrupt in ring3 so the dpt should be 3,
  // otherwise we have no privilege to access this descriptor.
  idt_entry_set(&idt[0x80], (uint64_t)sysint, 0xee);

  idt_ptr->limit = sizeof(idt) - 1;
  idt_ptr->base = (uint64_t)idt;
  load_idt(idt_ptr);
}