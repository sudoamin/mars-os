#include "proc.h"

#include "../../include/debug.h"
#include "../../include/string.h"
#include "../console/print.h"
#include "../int/idt.h"
#include "../mem/mem.h"

// tss.asm
extern struct TSS TSS;

static struct proc proces[NUM_PROC];
// pid_num is used to allocate a new proccess
// with a identification number
static int pid_num = 1;

// sets the top of kernel stack to rsp0 in tss
// when we jump from ring3 to ring0,
// the kernel stack is used
static void set_tss(struct proc *proc) { TSS.rsp0 = proc->kstack + STACK_SIZE; }

// find_unused_process finds an unused process in the ps table
static struct proc *find_unused_process(void) {
  struct proc *p = NULL;

  for (int i = 0; i < NUM_PROC; i++) {
    if (proces[i].state == PROC_UNUSED) {
      p = &proces[i];
      break;
    }
  }

  return p;
}

// set_process_entry sets ps structure
static void set_process_entry(struct proc *proc) {
  uint64_t stack_top;

  proc->state = PROC_INIT;
  proc->pid = pid_num++;

  // allocate a page for the kernel stack
  // so each ps has its own kernel stack
  proc->kstack = (uint64_t)kalloc();
  ASSERT(proc->kstack != 0);
  memset((void *)proc->kstack, 0, PAGE_SIZE);

  // because the stack grows downwards
  stack_top = proc->kstack + STACK_SIZE;

  // the base address of tram_frame
  // tram_frame is located at top of kernel stack (high address)
  proc->tf = (struct trap_frame *)(stack_top - sizeof(struct trap_frame));
  // they will be popped to the segment register and
  // other general purpose registers when
  // ireq instruction executes
  proc->tf->cs = 0x10 | 3;
  proc->tf->ss = 0x18 | 3;
  proc->tf->rflags = 0x202;
  // the code and stack of ps are in the same page
  proc->tf->rip = 0x400000;
  proc->tf->rsp = 0x400000 + PAGE_SIZE;

  // create a new kvm
  proc->pml4 = setup_kvm();
  ASSERT(proc->pml4 != 0);
  // create a new uvm
  // (uint64_t)main is the start address of program
  // and then the size of program
  // 5120 is 10 sectors
  ASSERT(setup_uvm(proc->pml4, (uint64_t)P2V(0x20000), 5120));
}

// init_process finds an unused process in the process table
void init_proc(void) {
  struct proc *proc = find_unused_process();
  ASSERT(proc == &proces[0]);

  set_process_entry(proc);
}

void launch(void) {
  set_tss(&proces[0]);
  // we are at the ps virtual space and have copied the main func (program)
  // in the address 400000
  switch_vm(proces[0].pml4);
  // jump to interrupt return to get ring3
  // and run the program
  // rdi = tf
  // and the values are in stack and will be popped to registers
  pstart(proces[0].tf);
}
