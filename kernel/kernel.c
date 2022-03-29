#include "kernel.h"

#include "../include/mem.h"

void kinit(void) {
  cosnole_init();
  idt_init();
  mem_init();
}

void kmain(void) {
  printf("Welcome to MarsOS \n");

  // ASSERT(0);
}