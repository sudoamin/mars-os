#include "../include/console.h"
#include "../include/debug.h"
#include "int/idt.h"

void kinit(void) {}

void kmain(void) {
  cosnole_init();
  idt_init();

  print("Welcome to MarsOS \n");
  
  // ASSERT(0);
}