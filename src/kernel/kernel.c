#include "kernel.h"

#include "../idt/idt.h"
#include "../terminal/terminal.h"

void init(void) {}

int main(void) {
  terminal_init();
  idt_init();

  print("Mars-os started :)");

  return 0;
}