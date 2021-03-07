#include "../include/string.h"

// This utility searches s1 for the presence of s2. If the string array s2 is found in the string
// arrays s1, the utility will return a pointer to the location of s2. Otherwise, it will return zero.
char *strstr(char *s1, const char *s2) {
    size_n n = strlen(s2);
    while (*s1)
        if (!memcmpt(s1++, s2, n))
            return s1 - 1;
    return 0;
}