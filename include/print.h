#ifndef PRINT_H
#define PRINT_H

#define LINE_SIZE 160

struct screen_bf {
  char *buffer;
  int column;
  int row;
};

int printf(const char *format, ...);
void write_screen(const char *buffer, int size, char color);
void clear_screen();

#endif