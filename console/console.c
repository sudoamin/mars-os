#include "../include/console.h"

uint16_t* video_mem = 0;
uint16_t cosnole_row = 0;
uint16_t console_col = 0;

void print(const char* str) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++) {
    console_writechar(str[i], 15);
  }
}

uint16_t console_make_char(char c, char color) { return (color << 8) | c; }

void cosnole_putchar(int x, int y, char c, char color) {
  video_mem[(y * VGA_WIDTH) + x] = console_make_char(c, color);
}

void console_writechar(char c, char color) {
  if (c == '\n') {
    cosnole_row += 1;
    console_col = 0;
    return;
  }

  cosnole_putchar(console_col, cosnole_row, c, color);
  console_col += 1;
  if (console_col >= VGA_WIDTH) {
    console_col = 0;
    cosnole_row += 1;
  }
}

void cosnole_init() {
  video_mem = (uint16_t*)(0xB8000);
  cosnole_row = 0;
  console_col = 0;
  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      cosnole_putchar(x, y, ' ', 0);
    }
  }
}
