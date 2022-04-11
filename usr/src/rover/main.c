#include <include/console.h>
#include <include/string.h>
#include <stdint.h>

typedef void (*cmd_func_t)(void);

static void execute_cmd(int cmd);
static int parse_cmd(char *buffer, int buffer_size);
static int read_cmd(char *buffer);
static void cmd_hello_world(void);

void main(void) {
  char buffer[80] = {0};
  int buffer_size = 0;
  int cmd = 0;

  while (1) {
    printf("rover@mars: ");
    buffer_size = read_cmd(buffer);

    if (buffer_size == 0) {
      continue;
    }

    cmd = parse_cmd(buffer, buffer_size);

    if (cmd < 0) {
      printf("command not found\n");
    } else {
      execute_cmd(cmd);
    }
  }
}

static void cmd_hello_world(void) {
  printf("Hello World\n");
}

static int read_cmd(char *buffer) {
  char ch[2] = {0};
  int buffer_size = 0;

  while (1) {
    ch[0] = sys_kbrd_read();

    if (ch[0] == '\n' || buffer_size >= 80) {
      printf("%s", ch);
      break;
    } else if (ch[0] == '\b') {
      if (buffer_size > 0) {
        buffer_size--;
        printf("%s", ch);
      }
    } else {
      buffer[buffer_size++] = ch[0];
      printf("%s", ch);
    }
  }

  return buffer_size;
}

static int parse_cmd(char *buffer, int buffer_size) {
  int cmd = -1;

  if (!memcmp("hello", buffer, buffer_size)) {
    cmd = 0;
  }

  return cmd;
}

static void execute_cmd(int cmd) {
  cmd_func_t cmd_list[1] = {cmd_hello_world};

  if (cmd == 0) {
    cmd_list[0]();
  }
}