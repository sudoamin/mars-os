#include "kernel.h"

#include "../console/console.h"
#include "../include/debug.h"
#include "../include/print.h"
#include "int/idt.h"
#include "mem/mem.h"

void kinit(void) {
  init_console();
  init_idt();
  init_mem();
  init_kvm();
}

void kmain(void) {
  printf("Welcome to MarsOS \n\n");

  printf("total free memory: %uMB \n", get_free_mem() / 1024 / 1024);

  // ASSERT(0);
}