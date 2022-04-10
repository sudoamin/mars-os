#include <include/console.h>
#include <include/string.h>
#include <include/syscall.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int printf(const char *format, ...) {
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

  buffer_size = sys_print(buffer, buffer_size);
  va_end(args);

  return buffer_size;
}