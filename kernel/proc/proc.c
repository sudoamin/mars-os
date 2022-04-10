#include <include/debug.h>
#include <include/string.h>
#include <kernel/include/idt.h>
#include <kernel/include/mem.h>
#include <kernel/include/proc.h>
#include <stddef.h>

#include "list.c"

// tss.asm
extern struct TSS TSS;

static struct proc proces[NUM_PROC];
// pid_num is used to allocate a new proccess
// with a identification number
static int pid_num = 1;

static struct ProcessControl pc;

static void set_tss(struct proc *proc) { TSS.rsp0 = proc->kstack + STACK_SIZE; }

static struct proc *find_unused_process(void) {
  struct proc *process = NULL;

  for (int i = 0; i < NUM_PROC; i++) {
    if (proces[i].state == PROC_UNUSED) {
      process = &proces[i];
      break;
    }
  }

  return process;
}

static void set_process_entry(struct proc *proc, uint64_t addr) {
  uint64_t stack_top;

  proc->state = PROC_INIT;
  proc->pid = pid_num++;

  proc->kstack = (uint64_t)kalloc();
  ASSERT(proc->kstack != 0);

  memset((void *)proc->kstack, 0, PAGE_SIZE);
  stack_top = proc->kstack + STACK_SIZE;

  proc->context = stack_top - sizeof(struct trap_frame) - 7 * 8;
  *(uint64_t *)(proc->context + 6 * 8) = (uint64_t)int_return;

  proc->tf = (struct trap_frame *)(stack_top - sizeof(struct trap_frame));
  proc->tf->cs = 0x10 | 3;
  proc->tf->rip = 0x400000;
  proc->tf->ss = 0x18 | 3;
  proc->tf->rsp = 0x400000 + PAGE_SIZE;
  proc->tf->rflags = 0x202;

  proc->pml4 = setup_kvm();
  ASSERT(proc->pml4 != 0);
  ASSERT(setup_uvm(proc->pml4, P2V(addr), 5120));
  proc->state = PROC_READY;
}

static struct ProcessControl *get_pc(void) { return &pc; }

void init_proc(void) {
  struct ProcessControl *process_control;
  struct proc *process;
  struct HeadList *list;
  uint64_t addr[2] = {0x20000, 0x30000};

  process_control = get_pc();
  list = &process_control->ready_list;

  for (int i = 0; i < 2; i++) {
    process = find_unused_process();
    set_process_entry(process, addr[i]);
    append_list_tail(list, (struct List *)process);
  }
}

void launch(void) {
  struct ProcessControl *process_control;
  struct proc *process;

  process_control = get_pc();
  process = (struct proc *)remove_list_head(&process_control->ready_list);
  process->state = PROC_RUNNING;
  process_control->current_process = process;

  set_tss(process);
  switch_vm(process->pml4);
  pstart(process->tf);
}

static void switch_process(struct proc *prev, struct proc *current) {
  set_tss(current);
  switch_vm(current->pml4);
  swap(&prev->context, current->context);
}

static void schedule(void) {
  struct proc *prev_proc;
  struct proc *current_proc;
  struct ProcessControl *process_control;
  struct HeadList *list;

  process_control = get_pc();
  prev_proc = process_control->current_process;
  list = &process_control->ready_list;
  ASSERT(!is_list_empty(list));

  current_proc = (struct proc *)remove_list_head(list);
  current_proc->state = PROC_RUNNING;
  process_control->current_process = current_proc;

  switch_process(prev_proc, current_proc);
}

void yield(void) {
  struct ProcessControl *process_control;
  struct proc *process;
  struct HeadList *list;

  process_control = get_pc();
  list = &process_control->ready_list;

  if (is_list_empty(list)) {
    return;
  }

  process = process_control->current_process;
  process->state = PROC_READY;
  append_list_tail(list, (struct List *)process);
  schedule();
}

void sleep(int wait) {
  struct ProcessControl *process_control;
  struct proc *process;

  process_control = get_pc();
  process = process_control->current_process;
  process->state = PROC_SLEEP;
  process->wait = wait;

  append_list_tail(&process_control->wait_list, (struct List *)process);
  schedule();
}

void wake_up(int wait) {
  struct ProcessControl *process_control;
  struct proc *process;
  struct HeadList *ready_list;
  struct HeadList *wait_list;

  process_control = get_pc();
  ready_list = &process_control->ready_list;
  wait_list = &process_control->wait_list;
  process = (struct proc *)remove_list(wait_list, wait);
  // generally, we could have multiple processes waiting on the same object
  // find all the waiting processes in the list
  while (process != NULL) {
    process->state = PROC_READY;
    append_list_tail(ready_list, (struct List *)process);
    process = (struct proc *)remove_list(wait_list, wait);
  }
}

void exit(void) {
  struct ProcessControl *process_control;
  struct proc *process;
  struct HeadList *list;

  process_control = get_pc();
  process = process_control->current_process;
  process->state = PROC_KILLED;

  list = &process_control->kill_list;
  append_list_tail(list, (struct List *)process);

  wake_up(1);
  schedule();
}

void wait(void) {
  struct ProcessControl *process_control;
  struct proc *process;
  struct HeadList *list;

  process_control = get_pc();
  list = &process_control->kill_list;

  while (1) {
    if (!is_list_empty(list)) {
      process = (struct proc *)remove_list_head(list);
      ASSERT(process->state == PROC_KILLED);

      kfree(process->kstack);
      free_vm(process->pml4);
      memset(process, 0, sizeof(struct proc));
    } else {
      sleep(1);
    }
  }
}