#include <include/debug.h>
#include <include/string.h>
#include <kernel/include/console.h>
#include <kernel/include/idt.h>
#include <kernel/include/mem.h>
#include <kernel/include/proc.h>
#include <stddef.h>

#include "list.c"
#include "tss.c"

// the programs are loaded into memory when we are in protected mode
// TODO, load programs from the boot driver or another locations
paddr_t programs[2] = {0x20000, 0x30000};

// because we have not heap
static struct proc ps_list[MAX_PROC_NUM];

static int last_pid = 1;

static struct proc *current_ps;
static struct list ready_list;
static struct list wait_list;
static struct list kill_list;

static struct proc *new_proc(paddr_t code);
static void switch_process(struct proc *prev, struct proc *current);
static void schedule(void);

// init_proc initializes and executes first programs
void init_proc(void) {
  // convert programs to processes and append them to ready_list for executing
  for (int i = 0; i < sizeof(programs) / sizeof(paddr_t); i++) {
    struct proc *p = new_proc(programs[i]);
    if (p == -1) {
      // TODO, LOG, not enough process to load all programs
    }

    ready_list = *list_append(&ready_list, p);
  }

  // execute first program

  if (list_is_empty(&ready_list)) {
    return;
  }

  struct proc *ps = ready_list.head;
  ps->state = PROC_RUNNING;
  current_ps = ps;

  ready_list = *list_remove_head(&ready_list);

  set_tss(ps);
  switch_vm(ps->pml4);
  pstart(ps->tf);
}

// code is the physical address of the program (RAM)
static struct proc *new_proc(paddr_t code) {
  // because we have not heap
  struct proc *ps;
  for (int i = 0; i < MAX_PROC_NUM; i++) {
    if (ps_list[i].state == PROC_UNUSED) {
      ps = &ps_list[i];
      break;
    }
  }

  // change process state and set process ID
  ps->state = PROC_INIT;
  ps->pid = last_pid;
  last_pid += 1;

  // A process has two stacks
  // one is for kernel mode and another is for user mode

  // for the kernel stack
  vaddr_t page = (vaddr_t)kalloc();
  if (page == 0) {
    // TODO, LOG
    return -1;
  }
  memset((void *)page, 0, PAGE_SIZE);
  ps->kstack = page;
  vaddr_t kstack_top = ps->kstack + STACK_SIZE;

  ps->context = kstack_top - sizeof(struct trap_frame) - 7 * 8;
  *(vaddr_t *)(ps->context + 6 * 8) = (vaddr_t)int_return;

  // tf holds the start address of the trap_frame structure
  ps->tf = (struct trap_frame *)(kstack_top - sizeof(struct trap_frame));
  // these fields will be popped to the segment or general-purpose registers,
  // when the iretq instruction executes.

  ps->tf->cs = 0x10 | 3;
  ps->tf->ss = 0x18 | 3;
  ps->tf->rflags = 0x202;
  // the code
  ps->tf->rip = 0x400000;
  // the user stack
  ps->tf->rsp = 0x400000 + PAGE_SIZE;

  // Each process has own address space
  // the kernel space is the same among all the processes
  // the user space saves its own program instructions and data

  // create the new kernel space
  ps->pml4 = setup_kvm();
  if (ps->pml4 == 0) {
    // TODO, LOG
    return -1;
  }

  // create the user space
  // and remap the code physical address to the virtual user space
  // always the address of the viraul user space is 400000
  if (!setup_uvm(ps->pml4, P2V(code), 5120)) {
    // TODO, LOG
    return -1;
  }

  ps->state = PROC_READY;

  return ps;
}

// proc_context_switch is called in the kernel/int/int.c
// to context switching by the timer
void proc_contex_switch(void) {
  if (list_is_empty(&ready_list)) {
    return;
  }

  struct proc *ps = current_ps;
  ps->state = PROC_READY;
  ready_list = *list_append(&ready_list, ps);

  schedule();
}

static void switch_process(struct proc *prev, struct proc *current) {
  set_tss(current);
  switch_vm(current->pml4);
  swap(&prev->context, current->context);
}

static void schedule(void) {
  // ASSERT(!);
  if (list_is_empty(&ready_list)) {
    printk("151 proc.c");
    return;
  }

  struct proc *next_ps = ready_list.head;
  next_ps->state = PROC_RUNNING;

  ready_list = *list_remove_head(&ready_list);

  struct proc *prev_ps = current_ps;
  current_ps = next_ps;
  switch_process(prev_ps, next_ps);
}

void proc_sleep(int wait) {
  struct proc *ps = current_ps;
  ps->state = PROC_SLEEP;
  ps->wait = wait;
  wait_list = *list_append(&wait_list, ps);

  schedule();
}

void proc_wake_up(int wait) {
  if (list_is_empty(&wait_list)) {
    return;
  }

  struct proc *ps = list_find(&wait_list, wait);
  if (ps == NULL) {
    return;
  }
  wait_list = *list_remove(&wait_list, ps);
  // generally, we could have multiple processes waiting on the same object
  // find all the waiting processes in the list
  while (ps != NULL) {
    ps->state = PROC_READY;
    ready_list = *list_append(&ready_list, ps);

    ps = list_find(&wait_list, wait);
    if (ps == NULL) {
      return;
    }
    wait_list = *list_remove(&wait_list, ps);
  }
}

void proc_exit(void) {
  struct proc *ps = current_ps;
  ps->state = PROC_KILLED;

  kill_list = *list_append(&kill_list, ps);

  proc_wake_up(2);
  schedule();
}

void proc_wait(void) {
  while (1) {
    if (!list_is_empty(&kill_list)) {
      struct proc *ps = kill_list.head;
      ASSERT(ps->state == PROC_KILLED);

      kfree(ps->kstack);
      free_vm(ps->pml4);
      memset(ps, 0, sizeof(struct proc));
    } else {
      proc_sleep(2);
    }
  }
}