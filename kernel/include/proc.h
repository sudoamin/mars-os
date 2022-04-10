#ifndef _PROC_H
#define _PROC_H

#include <kernel/include/idt.h>
#include <stdbool.h>
#include <stdint.h>

struct proc {
  int pid;
  // why the proc is put into sleep and how to wake it up
  int wait;
  int state;
  // saves the rsp value when switching process
  uint64_t context;
  uint64_t pml4;
  uint64_t kstack;
  struct trap_frame *tf;
  struct node *next;
};

#define STACK_SIZE (2 * 1024 * 1024)
#define MAX_PROC_NUM 10

#define PROC_UNUSED 0
#define PROC_INIT 1
#define PROC_RUNNING 2
#define PROC_READY 3
#define PROC_SLEEP 4
#define PROC_KILLED 5

void init_proc(void);
void proc_contex_switch(void);

void proc_sleep(int wait);
void proc_wake_up(int wait);
void proc_exit(void);
void proc_wait(void);

// idt.asm
extern void swap(uint64_t *prev, uint64_t next);
extern void pstart(struct trap_frame *tf);

#endif