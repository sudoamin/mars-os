#ifndef _MEM_H
#define _MEM_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

// see bootloader/mem.asm
struct E820 {
  uint64_t address;
  uint64_t length;
  uint32_t type;
} __attribute__((packed));

struct page {
  struct page *next;
};

typedef uint64_t vaddr_t;
typedef uint64_t paddr_t;

typedef uint64_t *PML4;
typedef uint64_t *PDP;
typedef uint64_t *PD;

// the attributes of table entries
#define PTE_P 1
#define PTE_W 2
#define PTE_U 4
#define PTE_ENTRY 0x80

#define KERNEL_BASE 0xffff800000000000
#define PAGE_SIZE (2 * 1024 * 1024)

// PA_UP aligns the address to the next 2M boundary if it is not aligned
// >> 21) << 21) clears 21 bits of value and we get aligned address
#define PA_UP(v) ((((uint64_t)v + PAGE_SIZE - 1) >> 21) << 21)
// PA_DOWN aligns the address to previous 2M boundary
#define PA_DOWN(v) (((uint64_t)v >> 21) << 21)
// virtual address = physical address + 0xffff800000000000
#define P2V(p) ((uint64_t)(p) + KERNEL_BASE)
#define V2P(v) ((uint64_t)(v)-KERNEL_BASE)
// retrieve address of next level page or physical page
// entries in PML4 table and page directory pointer table have
// attributes within the lower 12 bits
// so clear the lower 12 bits of the entry to get correct address
#define PDE_ADDR(p) (((uint64_t)p >> 12) << 12)
// page table has attributes within 21 bits
// so clear 21 bits of address to get physical address
#define PTE_ADDR(p) (((uint64_t)p >> 21) << 21)

void init_mem(void);
void init_kvm(void);

void *kalloc(void);
void kfree(vaddr_t addr);

uint64_t get_free_mem(void);

void free_uvm(PML4 pml4);

PML4 setup_kvm(void);
bool setup_uvm(PML4 pml4, paddr_t program, int size);
bool map_pages(PML4 pml4, vaddr_t v, vaddr_t e, paddr_t page,
               uint32_t attribute);
void switch_vm(PML4 pml4);

// mem.asm
void load_cr3(PML4 pml4);

#endif