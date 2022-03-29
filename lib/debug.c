#include "../include/debug.h"

void check_error(char* file, uint64_t line) {
  printf("\n ASSERT %s:%u \n", file, line);
}
