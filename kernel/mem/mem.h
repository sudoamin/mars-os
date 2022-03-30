#ifndef _MEM_H
#define _MEM_H

#include "stdint.h"

// see bootloader/mem.asm
struct E820 {
  uint64_t address;
  uint64_t length;
  uint32_t type;
} __attribute__((packed));

struct page {
  struct page* next;
};

#define KERNEL_BASE 0xffff800000000000
#define PAGE_SIZE (2 * 1024 * 1024)

// PA_UP aligns the address to the next 2M boundary if it is not aligned
// >> 21) << 21) clears 21 bits of value and we get aligned address
#define PA_UP(v) ((((uint64_t)v + PAGE_SIZE-1) >> 21) << 21)
// PA_DOWN aligns the address to previous 2M boundary
#define PA_DOWN(v) (((uint64_t)v >> 21) << 21)
// virtual address = physical address + 0xffff800000000000
#define P2V(p) ((uint64_t)(p) + KERNEL_BASE)
#define V2P(v) ((uint64_t)(v) - KERNEL_BASE)

void mem_init(void);
void* kalloc(void);
void kfree(uint64_t v);
uint64_t get_free_mem(void);

#endif