#ifndef SYSCALL_H_
#define SYSCALL_H_

#include "../int/idt.h"

typedef int (*SYSTEMCALL)(int64_t *argptr);

void init_syscall(void);
void syscall(struct trap_frame *tf);

#endif