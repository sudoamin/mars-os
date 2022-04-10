#ifndef CONSOLE_H
#define CONSOLE_H

void init_console();

#define LINE_SIZE 160

struct screen_bf {
  char *buffer;
  int column;
  int row;
};

int printk(const char *format, ...);
void console_write(const char *buffer, int size, char color);
void console_clear();

#endif