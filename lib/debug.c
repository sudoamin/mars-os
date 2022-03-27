#include "../include/debug.h"

void check_error(char* file, uint64_t line) {
  char* p = (char*)0xb8000;
  p[4] = 'E';
  p[5] = 0xa;
  while (1) {
  }
}
