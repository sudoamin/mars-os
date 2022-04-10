#include <include/debug.h>
#include <kernel/include/console.h>
#include <kernel/include/proc.h>
#include <kernel/include/syscall.h>
#include <stddef.h>

static int sys_console_print(int64_t *argptr);
static int sys_proc_sleep(int64_t *argptr);
static int sys_proc_exit(int64_t *argptr);
static int sys_proc_wait(int64_t *argptr);

static syscall_t syscalls[10];

void init_syscall(void) {
  syscalls[0] = sys_console_print;
  syscalls[1] = sys_proc_sleep;
  syscalls[2] = sys_proc_exit;
  syscalls[3] = sys_proc_wait;
}

void syscall(struct trap_frame *tf) {
  // the index number of syscall
  int64_t i = tf->rax;
  int64_t param_count = tf->rdi;
  // points to the args we passed to the function
  int64_t *argptr = (int64_t *)tf->rsi;

  if (param_count < 0) {
    // the error code
    tf->rax = -1;
    return;
  }

  ASSERT(syscalls[i] != NULL);
  // find syscall by the index,
  // pass the parameters and execute them
  // the rax holds the error code
  tf->rax = syscalls[i](argptr);
}

// when we call print function in the user program,
// we just convert the message to the string,
// prepare the string as argument on the user stack
// and execute int instruction

// argptr is the data on the stack in user mode
static int sys_console_print(int64_t *argptr) {
  console_write((char *)argptr[0], (int)argptr[1], 0xe);
  return (int)argptr[1];
}

static int sys_proc_sleep(int64_t *argptr) {
  uint64_t old_ticks;
  uint64_t ticks;
  uint64_t sleep_ticks = argptr[0];

  ticks = get_ticks();
  old_ticks = ticks;

  while (ticks - old_ticks < sleep_ticks) {
    proc_sleep(-1);
    ticks = get_ticks();
  }
  return 0;
}

static int sys_proc_exit(int64_t *argptr) {
  proc_exit();
  return 0;
}

static int sys_proc_wait(int64_t *argptr) {
  proc_wait();
  return 0;
}