#include "../../include/print.h"
#include "idt.h"

// int_handler will be called when we jump from ring3 to ring0.
// when an interrupt or exception occours
void int_handler(struct trap_frame *tf) {
  // unsigned char isr_value;

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
      printf("[Error %d at ring %d] %d:%x %x", tf->trapno, (tf->cs & 3),
             tf->errorcode, read_cr2(), tf->rip);
      while (1) {
      }
  }
}