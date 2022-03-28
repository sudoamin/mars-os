#ifndef PRINT_H
#define PRINT_H

#include "../include/mem.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdint.h"

#define LINE_SIZE 160

struct screen_bf {
  char *buffer;
  int column;
  int row;
};

int printk(const char *format, ...);
void write_screen(const char *buffer, int size, char color);
void clear_screen();

#endif