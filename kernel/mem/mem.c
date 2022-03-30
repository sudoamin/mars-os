#include "mem.h"

#include "../../include/debug.h"
#include "../../include/print.h"
#include "stddef.h"

// free pages in a linked list
static struct page page;
// end of memory, because the page linked list is reverse
// (uint64_t)page.next + PAGE_SIZE
// end of kernel, defined in linker.ld
extern char end;
static uint64_t total_free_mem = 0;

static void init_free_region(uint64_t v, uint64_t e);

void mem_init(void) {
  // E820 structure is initialized in bootloader/mem.asm
  // load e820 structures
  int32_t count = *(int32_t *)0x7e00;
  struct E820 *mem_map = (struct E820 *)0x7e08;

  ASSERT(count <= 50);

  for (int32_t i = 0; i < count; i++) {
    if (mem_map[i].type == 1) {  // it means this region is free
      total_free_mem += mem_map[i].length;

      uint64_t vstart = P2V(mem_map[i].address);
      uint64_t vend = vstart + mem_map[i].length;

      // if start of memory region is larger than end of kernel,
      // this is the free memory we want and do init.
      // if start of memory region is less than end of kernel,
      // it will compare end of region with end of kernel
      // if we is larger than end of kernel, we will initialize the memory from
      // end of kernel to end of region
      // otherwise we ignore the region because it is within the kernel
      // the end is a symbol instead of a variable (&end get address of symbol)
      if (vstart > (uint64_t)&end) {
        init_free_region(vstart, vend);
      } else if (vend > (uint64_t)&end) {
        init_free_region((uint64_t)&end, vend);
      }
    }
    // printf("%x  %uKB  %u\n", mem_map[i].address, mem_map[i].length / 1024,
    //         (uint64_t)mem_map[i].type);
  }
}

// init_free_region divids the region into 2MB pages and collect the pages
static void init_free_region(uint64_t v, uint64_t e) {
  // aligning the address to next 2MB boundary
  // then compare this page with end of the region
  // if it is within the region, call kfree
  for (uint64_t start = PA_UP(v); start + PAGE_SIZE <= e; start += PAGE_SIZE) {
    // 0xffff800040000000 is 1G above the base of kernel
    // if the page we are about to initialize is beyond the first 1G of RAM,
    // call kfree Note: we only use the first 1G of RAM
    if (v + PAGE_SIZE <= 0xffff800040000000) {
      kfree(start);
    }
  }
}

void kfree(uint64_t v) {
  // make sure the virtual address is page aligned
  ASSERT(v % PAGE_SIZE == 0);
  // make sure the virtual address is not within the kernel
  ASSERT(v >= (uint64_t)&end);
  // checks 1G memory limits
  ASSERT(v + PAGE_SIZE <= 0xffff800040000000);

  struct page *page_address = (struct page *)v;
  page_address->next = page.next;
  page.next = page_address;
}

// allocating memory is removing a page from the page linked list
void *kalloc(void) {
  struct page *page_address = page.next;

  if (page_address != NULL) {
    ASSERT((uint64_t)page_address % PAGE_SIZE == 0);
    ASSERT((uint64_t)page_address >= (uint64_t)&end);
    ASSERT((uint64_t)page_address + PAGE_SIZE <= 0xffff00040000000);

    page.next = page_address->next;
  }

  return (void *)page_address;
}

// get_free_mem returns total free memory in bytes
uint64_t get_free_mem(void) { return total_free_mem; }