#ifndef _KERNEL_H
#define _KERNEL_H

#include "../console/console.h"
#include "../include/debug.h"
#include "../include/print.h"
#include "int/idt.h"

void kinit();
void kmain();

#endif