#include <include/console.h>
#include <include/syscall.h>

void main(void) {
  // char *p = (char *)0xffff800000200200;
  // *p = 1;

  // int i = 0;
  // while (1) {
  // printf("%d ps1 executed\n", i);
  //   syscall_sleep(100);
  //   i++;
  // }

  // while (1) {
  // }
  // sys_wait();

  printf("\nps2\n");

  char ch[2] = {0};
  while (1) {
    ch[0] = sys_kbrd_read();
    printf("%s", ch);
  }
}