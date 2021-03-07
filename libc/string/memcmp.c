#include "../include/string.h"

// This utility accepts two memory areas and compares the first n byte of the two.
int memcmp(const void *aptr, const void *bptr, size_t size) {
    const unsigned char *a = (const unsigned char *) aptr;
    const unsigned char *b = (const unsigned char *) bptr;
    size_t i;
    for (int i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}
