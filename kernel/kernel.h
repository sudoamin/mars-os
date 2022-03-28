#ifndef KERNEL_H
#define KERNEL_H

#include "../console/console.h"
#include "../include/debug.h"
#include "../include/print.h"
#include "int/idt.h"

void kinit();
void kmain();

#endif