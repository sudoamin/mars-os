#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <kernel/include/idt.h>

typedef int (*syscall_t)(int64_t *argptr);

void init_syscall(void);
void syscall(struct trap_frame *tf);

#endif