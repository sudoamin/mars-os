#include "../../include/string.h"

#include "stdint.h"

// Searches s1 for the presence of s2. If the string array s2 is found in the
// string arrays s1, the utility will return a pointer to the location of s2.
// Otherwise, it will return zero.
char *strstr(char *s1, const char *s2) {
  size_t n = strlen(s2);
  while (*s1)
    if (!memcmp(s1++, s2, n)) return s1 - 1;
  return 0;
}

// Similar to the strcmp, this utility compares two string array as well.
// However, this utility accepts a third parameter that tells the utility to
// compare the first n characters of each string.
int strncmp(const char *s1, const char *s2, size_t n) {
  while (n--)
    if (*s1++ != *s2++)
      return *(unsigned char *)(s1 - 1) - *(unsigned char *)(s2 - 1);
  return 0;
}

// Accepts a character array and returns the length of that string.
size_t strlen(const char *str) {
  size_t ret = 0;
  while (str[ret] != 0) ret++;
  return ret;
}

// Accepts two string arrays and compares them. If the return value is less than
// zero, it indicates that s1 is less than s2. If the return value is greater
// than zero, it indicates s2 is less than s1. If the return value is equal to
// zero, it indicates that the two string arrays are the same.
int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s2)) s1++, s2++;
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// Accepts a string array and a character (represented as an int)
// and searches the string array for the existence of the character. If the
// character is found within is found within the string array, a pointer to that
// character's location will be returned. Otherwise, it will return NULL.
char *strchr(const char *s, int c) {
  while (*s != (char)c)
    if (!*s++) return 0;
  return (char *)s;
}

// Takes the string array src and appends it to the end of the dest string array
// and returns that concatenated value.
char *strcat(char *dest, const char *src) {
  char *tmp = dest;
  while (*dest) dest++;
  while ((*dest++ = *src++) != 0)
    ;

  return tmp;
}

// // Fills the first size bytes of memory pointed to
// // by bufptr with the constant byte value.
// void *memset(void *bufptr, int value, size_t size) {
//   unsigned char *buf = (unsigned char *)bufptr;
//   size_t i;
//   for (i = 0; i < size; i++) buf[i] = (unsigned char)value;
//   return bufptr;
// }

// // Accepts two memory areas and compares the first n byte of the two.
// int memcmp(const void *aptr, const void *bptr, size_t size) {
//   const unsigned char *a = (const unsigned char *)aptr;
//   const unsigned char *b = (const unsigned char *)bptr;
//   size_t i;
//   for (int i = 0; i < size; i++) {
//     if (a[i] < b[i])
//       return -1;
//     else if (b[i] < a[i])
//       return 1;
//   }
//   return 0;
// }

// Converts a single character into a NULL terminated string.
char *ctos(char s[2], const char c) {
  s[0] = c;
  s[1] = '\0';
  return s;
}

int isupper(char c) { return (c >= 'A' && c <= 'Z'); }

int islower(char c) { return (c >= 'a' && c <= 'z'); }

int isalpha(char c) {
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isspace(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\12');
}

int isdigit(char c) { return (c >= '0' && c <= '9'); }

char *ltrim(char *s) {
  while (isspace(*s)) s++;
  return s;
}

char *rtrim(char *s) {
  char *back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  return s;
}

char *trim(char *s) { return rtrim(ltrim(s)); }

int udecimal_to_string(char *buffer, int position, uint64_t digits) {
  char digits_map[10] = "0123456789";
  char digits_buffer[25];
  int size = 0;

  do {
    digits_buffer[size++] = digits_map[digits % 10];
    digits /= 10;
  } while (digits != 0);

  for (int i = size - 1; i >= 0; i--) {
    buffer[position++] = digits_buffer[i];
  }

  return size;
}

int decimal_to_string(char *buffer, int position, int64_t digits) {
  int size = 0;

  if (digits < 0) {
    digits = -digits;
    buffer[position++] = '-';
    size = 1;
  }

  size += udecimal_to_string(buffer, position, (uint64_t)digits);
  return size;
}

int hex_to_string(char *buffer, int position, uint64_t digits) {
  char digits_buffer[25];
  char digits_map[16] = "0123456789ABCDEF";
  int size = 0;

  do {
    digits_buffer[size++] = digits_map[digits % 16];
    digits /= 16;
  } while (digits != 0);

  for (int i = size - 1; i >= 0; i--) {
    buffer[position++] = digits_buffer[i];
  }

  buffer[position++] = 'H';

  return size + 1;
}

int read_string(char *buffer, int position, const char *string) {
  int index = 0;

  for (index = 0; string[index] != '\0'; index++) {
    buffer[position++] = string[index];
  }

  return index;
}