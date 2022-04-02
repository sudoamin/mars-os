#ifndef STRING_H
#define STRING_H

#include <stdint.h>

#include "stddef.h"

void memset(void *buffer, char value, int size);
void memmove(void *dst, void *src, int size);
void memcpy(void *dst, void *src, int size);
int memcmp(void *src1, void *src2, int size);

size_t strlen(const char *);
char *strcat(char *d, const char *s);
char *strcpy(char *d, const char *s);
int strcmp(const char *s1, const char *s2);
char *strncat(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strstr(char *s1, const char *s2);
char *strchr(const char *s, int c);
int strncmp(const char *s1, const char *s2, size_t n);

int udecimal_to_string(char *buffer, int position, uint64_t digits);
int decimal_to_string(char *buffer, int position, int64_t digits);
int hex_to_string(char *buffer, int position, uint64_t digits);
int read_string(char *buffer, int position, const char *string);

#endif