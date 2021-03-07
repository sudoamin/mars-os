#include "../include/string.h"
#include "strdef.h"

// Similar to the strcmp utility, this utility compares two string array as well.
// However, this utility accepts a third parameter that tells the utility to compare the first n characters of each string.
int strncmp(const char *s1, const char *s2, size_t n) {
    while (n--)
        if (*s1++ != *s2++)
            return *(unsigned char *) (s1 - 1) - *(unsigned char *) (s2 - 1);
    return 0;
}
