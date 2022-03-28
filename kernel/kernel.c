#include "kernel.h"

void kinit(void) {}

void kmain(void) {
  cosnole_init();
  idt_init();

  printk("Welcome to MarsOS \n");

  // ASSERT(0);
}