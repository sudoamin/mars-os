#ifndef STRING_H
#define STRING_H

#include "stddef.h"

size_t strlen(const char *);
char *strcat(char *d, const char *s);
char *strcpy(char *d, const char *s);
int strcmp(const char *s1, const char *s2);
char *strncat(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strstr(char *s1, const char *s2);
char *strchr(const char *s, int c);
int strncmp(const char *s1, const char *s2, size_t n);

#endif