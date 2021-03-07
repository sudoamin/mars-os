#include "../include/string.h"

// This utility fills the first size bytes of memory pointed to
// by bufptr with the constant byte value.
void *memset(void *bufptr, int value, size_t size) {
    unsigned char *buf = (unsigned char *) bufptr;
    size_t i;
    for (i = 0; i < size; i++)
        buf[i] = (unsigned char) value;
    return bufptr;
}
