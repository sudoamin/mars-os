#include "mem.h"

#include "../../include/debug.h"
#include "../../include/print.h"
#include "../../include/string.h"
#include "stdbool.h"
#include "stddef.h"

// free pages in a linked list
static struct page page;
// end of kernel, defined in linker.ld
extern char end;
static uint64_t total_free_mem = 0;
uint64_t mem_end;

static void init_free_region(uint64_t v, uint64_t e);

void init_mem(void) {
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
  // because the page linked list is reverse
  mem_end = (uint64_t)page.next + PAGE_SIZE;
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
    if (start + PAGE_SIZE <= 0xffff800040000000) {
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
    ASSERT((uint64_t)page_address + PAGE_SIZE <= 0xffff800040000000);

    page.next = page_address->next;
  }

  return (void *)page_address;
}

// get_free_mem returns total free memory in bytes
uint64_t get_free_mem(void) { return total_free_mem; }

// find_pml4t_entry finds the entry according to the virtual address
// the entry points to page directory pointer table
static PDPTR find_pml4t_entry(uint64_t map, uint64_t v, int alloc,
                              uint32_t attribute) {
  PDPTR *map_entry = (PDPTR *)map;
  PDPTR pdptr = NULL;
  unsigned int index = (v >> 39) & 0x1FF;

  // if the present bit is set, copy the address to pdpt
  if ((uint64_t)map_entry[index] & PTE_P) {
    pdptr = (PDPTR)P2V(PDE_ADDR(map_entry[index]));
  } else if (alloc == 1) {
    pdptr = (PDPTR)kalloc();
    if (pdptr != NULL) {
      memset(pdptr, 0, PAGE_SIZE);
      map_entry[index] = (PDPTR)(V2P(pdptr) | attribute);
    }
  }

  return pdptr;
}

// find_pdpt_entry finds the entry in the directory pointer table
// which points to the page directory table.
// alloc indicating whether or not we will create a page if it does exist
static PD find_pdpt_entry(uint64_t pml4, uint64_t vaddr, int alloc,
                          uint32_t attribute) {
  PDPTR pdptr = NULL;
  PD pd = NULL;
  // index is located from the bits 30 of the virtual address and
  // is also 9 bits in total
  unsigned int index = (vaddr >> 30) & 0x1FF;

  pdptr = find_pml4t_entry(pml4, vaddr, alloc, attribute);
  if (pdptr == NULL) return NULL;

  // find entry using index
  // if the present bit of attributes is 1, then it means that
  // the value in the entry points to next level table which is page directory
  // table
  if ((uint64_t)pdptr[index] & PTE_P) {
    // clear attribute bits to get address
    // convert physical address to virtual
    pd = (PD)P2V(PDE_ADDR(pdptr[index]));
  }
  // if the present bit is not set, then it is an unused entry
  // if alloc is 1, allocate a new page and set the entry to
  // make it point to this page
  else if (alloc == 1) {
    pd = (PD)kalloc();
    if (pd != NULL) {
      memset(pd, 0, PAGE_SIZE);
      pdptr[index] = (PD)(V2P(pd) | attribute);
    }
  }

  return pd;
}

// in the page directory table, we can set the corresponding entry to
// map the addresses to the physical page
// map_pages mapps the address to physical page
bool map_pages(uint64_t map, uint64_t v, uint64_t e, uint64_t pa,
               uint32_t attribute) {
  // aligned virtual addresses
  uint64_t vstart = PA_DOWN(v);
  uint64_t vend = PA_UP(e);

  PD pd = NULL;
  // locate the specific entry in the PD
  unsigned int index;

  // checks start and end of region
  ASSERT(v < e);
  // make sure the physical address we want to map into is page aligned
  ASSERT(pa % PAGE_SIZE == 0);
  // checks if end of physical address is outside the range of 1G memory
  ASSERT(pa + vend - vstart <= 1024 * 1024 * 1024);

  do {
    // find the page directory pointer table entry which
    // points to page directory table
    pd = find_pdpt_entry(map, vstart, 1, attribute);
    if (pd == NULL) {
      return false;
    }

    // index value is 9 bits in total starting from bits 21
    // so we shift right 21 bits and clear other bits excep the lower 9 bits of
    // the result
    index = (vstart >> 21) & 0x1FF;
    // if the present bit is set, it means that we
    // remap to the used page
    ASSERT(((uint64_t)pd[index] & PTE_P) == 0);

    // set entry
    pd[index] = (PDE)(pa | attribute | PTE_ENTRY);

    // move to the next page
    vstart += PAGE_SIZE;
    pa += PAGE_SIZE;

    // if the virtual address is still within the memory region,
    // we continue the process until we reach end of region
  } while (vstart + PAGE_SIZE <= vend);

  return true;
}

// switch_vm loads cr3 register with new PML4 table to make the mapping
void switch_vm(uint64_t map) { load_cr3(V2P(map)); }

// each process has its own address space,
// any changes in one address space will not affect other address spaces
// so we can setup kernel space and user space for each process

// setup_kvm remaps the kernel using 2MB pages and returns the new PML4 table
uint64_t setup_kvm(void) {
  uint64_t pml4 = (uint64_t)kalloc();
  if (pml4 != 0) {
    memset((void *)pml4, 0, PAGE_SIZE);
    // KERNEL_BASE is start of memory region
    // PTE_P | PTE_W specify that the kernel memory is readable, writable
    // and not accessible by user applications
    if (!map_pages(pml4, KERNEL_BASE, mem_end, V2P(KERNEL_BASE),
                   PTE_P | PTE_W)) {
      free_vm(pml4);
      pml4 = 0;
    }
  }

  return pml4;
}

void init_kvm(void) {
  uint64_t pml4 = setup_kvm();
  ASSERT(pml4 != 0);
  switch_vm(pml4);
}

// for freeing memory, we first locate the entry in the page directory table,
// and free the page using the physical address saved in the entry.
// when we want to free whole memory, we will free the physical pages first,
// then we free page directory tables, page directory pointer tables and PML4.
// Generally, we could have a few page directory pointer tables and page
// directory tables. so we need to loop through each of the tables.

// free_pages is like reverse process of mapping pages.
void free_pages(uint64_t map, uint64_t vstart, uint64_t vend) {
  // index is used to locate entry in the page directory table
  unsigned int index;

  // vstart and vend have to be aligned
  ASSERT(vstart % PAGE_SIZE == 0);
  ASSERT(vend % PAGE_SIZE == 0);

  do {
    // find the page directory table
    // alloc is 0 which means
    // find_pdpt_entry returns null if the entry does not exist
    // because we want to free the existing page
    PD pd = find_pdpt_entry(map, vstart, 0, 0);

    if (pd != NULL) {
      index = (vstart >> 21) & 0x1FF;
      // find corresponding entry and check present bit
      if (pd[index] & PTE_P) {
        // the lower 21 bits should be cleared before using the address.
        // PTE_ADDR
        kfree(P2V(PTE_ADDR(pd[index])));
        // the entry now is unused
        pd[index] = 0;
      }
    }

    // move to next page
    vstart += PAGE_SIZE;
    // continue process until we reach end of memory region
  } while (vstart + PAGE_SIZE <= vend);
}

// loop through the PML4 table and page directory pinter tables
// to find page directory tables.
static void free_pdt(uint64_t map) {
  PDPTR *pml4 = (PDPTR *)map;

  // since each entry in PML4 table points to page directory table,
  // we could have a total of 512 PDP tables

  // loop through each of the entries
  for (int i = 0; i < 512; i++) {
    // if the entry is present
    if ((uint64_t)pml4[i] & PTE_P) {
      // retrieve the address of the entry that are page directory pointer table
      PD *pdptr = (PD *)P2V(PDE_ADDR(pml4[i]));

      // the PDP tale also include 512 entries which
      // points to page directory tables
      for (int j = 0; j < 512; j++) {
        // if the entry is valid or present
        if ((uint64_t)pdptr[j] & PTE_P) {
          // clear the entry
          kfree(P2V(PDE_ADDR(pdptr[j])));
          pdptr[j] = 0;
        }
      }
    }
  }
}

// free_pdpt is used to free page directory pointer tables
static void free_pdpt(uint64_t map) {
  PDPTR *pml4 = (PDPTR *)map;

  for (int i = 0; i < 512; i++) {
    // if the entry is used, it frees the page pointed to by the entry
    // which is page directory pointer table
    if ((uint64_t)pml4[i] & PTE_P) {
      kfree(P2V(PDE_ADDR(pml4[i])));
      pml4[i] = 0;
    }
  }
}

static void free_pml4t(uint64_t map) { kfree(map); }

// to free vm we free the physical pages in the user space as
// well as page tables.
// because we need the page tables to locate the physical pages
// we first free the pages by calling free_pages
// then we free the page directory tables
// and page directory pointer tables.
// we do not free the kernel page
// because the kernel page is shared among all the vms
void free_vm(uint64_t map) {
  // free one page
  free_pages(map, 0x400000, 0x400000 + PAGE_SIZE);
  free_pdt(map);
  free_pdpt(map);
  free_pml4t(map);
}

// we map only one page for the user programs which means
// the code, data and stack of the programs are located in the same 2M page.

// start is the location of the program
// size is the size of the data we will copy
bool setup_uvm(uint64_t pml4, uint64_t data, int size) {
  // allocate a page which is used to
  // store the code and data of the program
  void *p = kalloc();
  bool status = false;

  if (p != NULL) {
    // it could include some random values
    memset(p, 0, PAGE_SIZE);
    // map the page using map_pages
    // because we implement only one page for user application,
    // we add page size to the base address
    // and next one is the base of physical page we want to map into
    // which is the page we allocate
    status = map_pages(pml4, 0x400000, 0x400000 + PAGE_SIZE, V2P(p),
                       PTE_P | PTE_W | PTE_U);
    if (status == true) {
      memcpy(p, (void *)data, size);
    } else {
      kfree((uint64_t)p);
      free_vm(pml4);
    }
  }

  return status;
}