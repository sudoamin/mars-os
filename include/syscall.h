#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

int sys_print(char* buffer, int buffer_size);
unsigned char sys_kbrd_read(void);
void sys_sleep(uint64_t ticks);
void sys_exit(void);
void sys_wait(void);

#endif