#ifndef _PROC_H
#define _PROC_H

#include <kernel/include/idt.h>

#include <stdbool.h>

struct List {
  struct List *next;
};

struct HeadList {
  struct List *next;
  struct List *tail;
};

bool is_list_empty(struct HeadList *list);

struct proc {
  struct List *next;
  // saves the rsp value when switching process
  uint64_t context;
  int pid;
  // why the proc is put into sleep and how to wake it up
  int wait;
  int state;
  uint64_t pml4;
  uint64_t kstack;
  struct trap_frame *tf;
};

struct TSS {
  uint32_t res0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t res1;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t res2;
  uint16_t res3;
  uint16_t iopb;
} __attribute__((packed));

struct ProcessControl {
  struct Process *current_process;
  struct HeadList ready_list;
  struct HeadList wait_list;
  struct HeadList kill_list;
};

#define STACK_SIZE (2 * 1024 * 1024)
#define NUM_PROC 10

#define PROC_UNUSED 0
#define PROC_INIT 1
#define PROC_RUNNING 2
#define PROC_READY 3
#define PROC_SLEEP 4
#define PROC_KILLED 5

void init_proc(void);
void launch(void);
void pstart(struct trap_frame *tf);
void yield(void);
void swap(uint64_t *prev, uint64_t next);
void sleep(int wait);
void wake_up(int wait);
void exit(void);
void wait(void);

#endif