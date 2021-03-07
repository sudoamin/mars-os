#include "../include/string.h"

// This utility accepts a string array and a character (represented as an int)
// and searches the string array for the existence of the character. If the character is found within is found within the string array,
// a pointer to that character's location will be returned. Otherwise, it will return NULL.
char *strchr(const char *s, int c) {
    while (*s != (char) c)
        if (!*s++)
            return 0;
    return (char *) s;
}