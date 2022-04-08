#ifndef SYSCALL_H
#define SYSCALL_H

#include "stdint.h"

int syscall_write(char* buffer, int buffer_size);
void syscall_sleep(uint64_t ticks);

#endif