#include "../../../include/console.h"
#include "../../../include/syscall.h"

void main(void) {
  int i = 0;
  while (1) {
    printf("%d ps1 executed\n", i);
    syscall_sleep(100);
    i++;
  }

  while (1) {
  }
}