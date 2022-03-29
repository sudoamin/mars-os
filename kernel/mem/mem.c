#include "mem.h"

#include "../../include/debug.h"
#include "../../include/print.h"
#include "stddef.h"

static void free_region(uint64_t v, uint64_t e);

static struct free_mem_region free_mem_region[50];
static struct page free_mem;
static uint64_t mem_end;
extern char end;

void mem_init(void) {
  int32_t count = *(int32_t *)0x8000;
  uint64_t total_mem = 0;
  struct E820 *mem_map = (struct E820 *)0x8008;
  int free_region_count = 0;

  ASSERT(count <= 50);

  for (int32_t i = 0; i < count; i++) {
    if (mem_map[i].type == 1) {
      free_mem_region[free_region_count].address = mem_map[i].address;
      free_mem_region[free_region_count].length = mem_map[i].length;

      total_mem += mem_map[i].length;
      free_region_count++;
    }
    printf("%x  %uKB  %u\n", mem_map[i].address, mem_map[i].length / 1024,
           (uint64_t)mem_map[i].type);
  }

  for (size_t i = 0; i < free_region_count; i++) {
    uint64_t vstart = P2V(free_mem_region[i].address);
    uint64_t vend = vstart + free_mem_region[i].length;

    if (vstart > (uint64_t)&end) {
      free_region(vstart, vend);
    } else if (vend > (uint64_t)&end) {
      free_region((uint64_t)&end, vend);
    }
  }

  mem_end = (uint64_t)free_mem.next + PAGE_SIZE;
  printf("end of memory: %x\n", mem_end);

  printf("total free memory: %uMB \n", total_mem / 1024 / 1024);
}

static void free_region(uint64_t v, uint64_t e) {
  for (uint64_t start = PA_UP(v); start + PAGE_SIZE <= e; start += PAGE_SIZE) {
    if (v + PAGE_SIZE <= 0xffff800040000000) {
      kfree(start);
    }
  }
}

void kfree(uint64_t v) {
  ASSERT(v % PAGE_SIZE == 0);
  ASSERT(v >= (uint64_t)&end);
  ASSERT(v + PAGE_SIZE <= 0xffff800040000000);

  struct page *page_address = (struct page *)v;
  page_address->next = free_mem.next;
  free_mem.next = page_address;
}

void *kalloc(void) {
  struct page *page_address = free_mem.next;

  if (page_address != NULL) {
    ASSERT((uint64_t)page_address % PAGE_SIZE == 0);
    ASSERT((uint64_t)page_address >= (uint64_t)&end);
    ASSERT((uint64_t)page_address + PAGE_SIZE <= 0xffff00040000000);

    free_mem.next = page_address->next;
  }

  return (void *)page_address;
}