#include "kernel.h"

#include <include/debug.h>
#include <kernel/include/console.h>
#include <kernel/include/idt.h>
#include <kernel/include/mem.h>
#include <kernel/include/proc.h>
#include <kernel/include/syscall.h>

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
  printk("total free memory: %uMB \n", get_free_mem() / 1024 / 1024);
}