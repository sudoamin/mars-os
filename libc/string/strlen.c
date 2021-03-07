#include "../include/string.h"

// This utility accepts a character array and returns the length of that string.
size_t strlen(const char *str) {
    size_t ret = 0;
    while (str[ret] != 0)
        ret++;
    return ret;
}
