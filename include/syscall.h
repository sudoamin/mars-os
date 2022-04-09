#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

int sys_print(char* buffer, int buffer_size);
void sys_sleep(uint64_t ticks);
void sys_exit(void);
void sys_wait(void);

#endif