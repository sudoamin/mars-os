#include "../include/debug.h"
#include "int/idt.h"

void kinit(void) {
  char* p = (char*)0xb8000;
  p[52] = 'i';
  p[53] = 0xa;
}

int kmain(void) {
  char* p = (char*)0xb8000;
  p[50] = 'c';
  p[51] = 0xa;

  idt_init();

  ASSERT(0);

  return 0;
}