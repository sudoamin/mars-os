#ifndef DEBUG_H
#define DEBUG_H

#include "../include/console.h"
#include "stdint.h"

#define ASSERT(e)                              \
  do {                                         \
    if (!(e)) check_error(__FILE__, __LINE__); \
  } while (0)

void check_error(char *file, uint64_t line);

#endif