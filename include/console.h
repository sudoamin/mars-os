#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#include "../include/string.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void cosnole_init();
void print(const char* str);

#endif