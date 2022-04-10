#include <kernel/include/idt.h>
#include <kernel/include/console.h>
#include <kernel/include/proc.h>
#include <kernel/include/syscall.h>

static uint64_t ticks;

uint64_t get_ticks(void) { return ticks; }

static void timer_handler(void) {
  ticks++;
  proc_wake_up(-1);
}

// int_handler will be called when we jump from ring3 to ring0.
// when an interrupt or exception occours
void int_handler(struct trap_frame *tf) {
  // unsigned char isr_value;

  switch (tf->trapno) {
    case 32:
      timer_handler();
      eoi();
      break;

    case 39:
      unsigned char isr_value = read_isr();
      if ((isr_value & (1 << 7)) != 0) {
        eoi();
      }
      break;

    case 0x80:
      // tf to reference the data in the user stack
      syscall(tf);
      break;

    default:
      if ((tf->cs & 3) == 3) {
        printk("\nException is %d\n", tf->trapno);
        proc_exit();
        break;
      }
      printk("[Error %d at ring %d] %d:%x %x", tf->trapno, (tf->cs & 3),
             tf->errorcode, read_cr2(), tf->rip);
      while (1) {
      }
  }

  if (tf->trapno == 32) {
    proc_contex_switch();
  }
}