#include <kernel/include/proc.h>
#include <stdint.h>

// tss.asm
extern struct TSS TSS;

// TSS is used only for setting up stack pointer for Ring0
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

// set_tss sets the top of stack to rsp0 in the TSS.
// when the interrupt or exception handler is called, 
// the stack used is the kernel stack we set up in the process.
// or in another words when we jump from Ring3 to Ring0,
// the kernel stack is used
void set_tss(struct proc *proc) { TSS.rsp0 = proc->kstack + STACK_SIZE; }
