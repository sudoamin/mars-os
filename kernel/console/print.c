#include "print.h"

#include "../../include/string.h"
#include "../mem/mem.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdint.h"

static struct screen_bf screen_bf = {(char *)P2V(0xb8000), 0, 0};

void write_screen(const char *buffer, int size, char color) {
  struct screen_bf *sb = &screen_bf;
  int column = sb->column;
  int row = sb->row;

  for (int i = 0; i < size; i++) {
    if (buffer[i] == '\n') {
      column = 0;
      row++;
    } else if (buffer[i] == '\b') {
      if (column == 0 && row == 0) {
        continue;
      }

      if (column == 0) {
        row--;
        column = 80;
      }

      column -= 1;
      sb->buffer[column * 2 + row * LINE_SIZE] = 0;
      sb->buffer[column * 2 + row * LINE_SIZE + 1] = 0;
    } else {
      sb->buffer[column * 2 + row * LINE_SIZE] = buffer[i];
      sb->buffer[column * 2 + row * LINE_SIZE + 1] = color;
      column++;

      if (column >= 80) {
        column = 0;
        row++;
      }
    }

    if (row >= 25) {
      memcpy(sb->buffer, sb->buffer + LINE_SIZE, LINE_SIZE * 24);
      memset(sb->buffer + LINE_SIZE * 24, 0, LINE_SIZE);
      row--;
    }
  }

  sb->column = column;
  sb->row = row;
}

int printk(const char *format, ...) {
  char buffer[1024];
  int buffer_size = 0;
  int64_t integer = 0;
  char *string = 0;
  va_list args;

  va_start(args, format);

  for (int i = 0; format[i] != '\0'; i++) {
    if (format[i] != '%') {
      buffer[buffer_size++] = format[i];
    } else {
      switch (format[++i]) {
        case 'x':
          integer = va_arg(args, int64_t);
          buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer);
          break;

        case 'u':
          integer = va_arg(args, int64_t);
          buffer_size +=
              udecimal_to_string(buffer, buffer_size, (uint64_t)integer);
          break;

        case 'd':
          integer = va_arg(args, int64_t);
          buffer_size += decimal_to_string(buffer, buffer_size, integer);
          break;

        case 's':
          string = va_arg(args, char *);
          buffer_size += read_string(buffer, buffer_size, string);
          break;

        default:
          buffer[buffer_size++] = '%';
          i--;
      }
    }
  }

  write_screen(buffer, buffer_size, 0xf);
  va_end(args);

  return buffer_size;
}

void clear_screen() {
  for (size_t i = 0; i < 200; i++) {
    for (size_t i = 0; i < 200; i++) {
      printk(" ");
    }
    printk("\n");
  }

  struct screen_bf *sb = &screen_bf;
  sb->column = 0;
  sb->row = 0;
}