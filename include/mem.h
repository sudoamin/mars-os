#ifndef MEM_H
#define MEM_H

#include "stdint.h"

struct E820 {
  uint64_t address;
  uint64_t length;
  uint32_t type;
} __attribute__((packed));

struct free_mem_region {
  uint64_t address;
  uint64_t length;
};

struct page {
  struct page* next;
};

#define KERNEL_BASE 0xffff800000000000
#define PAGE_SIZE (2 * 1024 * 1024)

#define PA_UP(v) ((((uint64_t)v + PAGE_SIZE-1) >> 21) << 21)
#define PA_DOWN(v) (((uint64_t)v >> 21) << 21)
#define P2V(p) ((uint64_t)(p) + KERNEL_BASE)
#define V2P(v) ((uint64_t)(v) - KERNEL_BASE)

void mem_init(void);
void* kalloc(void);
void kfree(uint64_t v);

void memset(void* buffer, char value, int size);
void memmove(void* dst, void* src, int size);
void memcpy(void* dst, void* src, int size);
int memcmp(void* src1, void* src2, int size);

#endif