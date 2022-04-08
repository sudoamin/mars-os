#include "kernel.h"

#include "../include/debug.h"
#include "console/console.h"
#include "console/print.h"
#include "int/idt.h"
#include "mem/mem.h"
#include "proc/proc.h"
#include "syscall/syscall.h"

void kinit(void) {
  init_console();
  init_idt();
  init_mem();
  init_kvm();
  init_syscall();

  init_proc();
  launch();
}

void kmain(void) {
  //  printk("total free memory: %uMB \n", get_free_mem() / 1024 / 1024);

  // ASSERT(0);
}