#include "idt.h"

void int_handler(struct trap_frame *tf) {
//   unsigned char isr_value;

  switch (tf->trapno) {
    case 32:
      eoi();
      break;

    case 39:
      unsigned char isr_value = read_isr();
      if ((isr_value & (1 << 7)) != 0) {
        eoi();
      }
      break;

    default:
      while (1) {
      }
  }
}