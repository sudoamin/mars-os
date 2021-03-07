#include "../include/string.h"

// This utility converts a single character into a NULL terminated string.
char *ctos(char s[2], const char c) {
    s[0] = c;
    s[1] = '\0';
    return s;
}