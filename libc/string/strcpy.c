#include "../include/string.h"

// This utility accepts two string arrays and compares them. If the return value is less than zero,
// it indicates that s1 is less than s2. If the return value is greater than zero,
// it indicates s2 is less than s1. If the return value is equal to zero,
// it indicates that the two string arrays are the same.
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s2))
        s1++, s2++;
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}