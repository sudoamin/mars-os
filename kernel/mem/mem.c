#include "mem.h"

#include "../../include/debug.h"
#include "../../include/string.h"
#include "../console/print.h"
#include "stdbool.h"
#include "stddef.h"

// free pages in a linked list
static struct page page;
// end of kernel, defined in linker.ld
extern char end;
static uint64_t total_free_mem = 0;
vaddr_t mem_end;

static void init_free_region(vaddr_t s, vaddr_t e);

void init_mem(void) {
  // E820 structure is initialized in bootloader/mem.asm
  // load e820 structures
  int32_t count = *(int32_t *)0x8e00;
  struct E820 *mem_map = (struct E820 *)0x8e08;

  ASSERT(count <= 50);

  for (int32_t i = 0; i < count; i++) {
    if (mem_map[i].type == 1) {  // it means this region is free
      total_free_mem += mem_map[i].length;

      vaddr_t vstart = P2V(mem_map[i].address);
      vaddr_t vend = vstart + mem_map[i].length;

      // if start of memory region is larger than end of kernel,
      // this is the free memory we want and do init.
      // if start of memory region is less than end of kernel,
      // it will compare end of region with end of kernel
      // if we is larger than end of kernel, we will initialize the memory from
      // end of kernel to end of region
      // otherwise we ignore the region because it is within the kernel
      // the end is a symbol instead of a variable (&end get address of symbol)
      if (vstart > (vaddr_t)&end) {
        init_free_region(vstart, vend);
      } else if (vend > (vaddr_t)&end) {
        init_free_region((vaddr_t)&end, vend);
      }
    }
    // printk("%x  %uKB  %u\n", mem_map[i].address, mem_map[i].length / 1024,
    //         (uint64_t)mem_map[i].type);
  }
  // because the page linked list is reverse
  mem_end = (vaddr_t)page.next + PAGE_SIZE;
}

// init_free_region divids the region into 2MB pages and collect the pages
static void init_free_region(vaddr_t s, vaddr_t e) {
  // aligning the address to next 2MB boundary
  // then compare this page with end of the region
  // if it is within the region, call kfree
  for (vaddr_t start = PA_UP(s); start + PAGE_SIZE <= e; start += PAGE_SIZE) {
    // 0xffff800040000000 is 1G above the base of kernel
    // if the page we are about to initialize is beyond the first 1G of RAM,
    // call kfree Note: we only use the first 1G of RAM
    if (start + PAGE_SIZE <= 0xffff800040000000) {
      kfree(start);
    }
  }
}

void kfree(vaddr_t v) {
  // make sure the virtual address is page aligned
  ASSERT(v % PAGE_SIZE == 0);
  // make sure the virtual address is not within the kernel
  ASSERT(v >= (vaddr_t)&end);
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
    ASSERT((vaddr_t)page_address % PAGE_SIZE == 0);
    ASSERT((vaddr_t)page_address >= (vaddr_t)&end);
    ASSERT((vaddr_t)page_address + PAGE_SIZE <= 0xffff800040000000);

    page.next = page_address->next;
  }

  return (void *)page_address;
}

// get_free_mem returns total free memory in bytes
uint64_t get_free_mem(void) { return total_free_mem; }

// find_pml4t_entry finds the PDP
// the address of PDP is stored in the entry
// the index of the entry are stored in the addr from bits 39
static PDP find_pml4t_entry(PML4 pml4, vaddr_t addr, int alloc,
                            uint32_t attribute) {
  // entry without attributes
  vaddr_t entry = 0;
  // index is located from the bits 39 of the virtual address and
  // is also 9 bits in total
  unsigned int index = (addr >> 39) & 0x1FF;

  // if the present bit is set, copy the address to pdp
  if (pml4[index] & PTE_P) {
    // PDE_ADDR clears the attributes
    entry = P2V(PDE_ADDR(pml4[index]));
  } else if (alloc == 1) {
    // define new entry
    vaddr_t pdp = kalloc();
    if (pdp != NULL) {
      memset((void *)pdp, 0, PAGE_SIZE);
      // from bit 12 to .. is PDP table addres
      // bits 0=P 1=W 2=U for attributes
      pml4[index] = V2P(pdp) | attribute;
      entry = pdp;
    }
  }

  return (PDP)entry;
}

// find_pdpt_entry finds the PD
// alloc indicating whether or not we will create a page if it does exist
static PD find_pdp_entry(PDP pdp, vaddr_t addr, int alloc, uint32_t attribute) {
  PD pd = NULL;
  // index is located from the bits 30 of the virtual address and
  // is also 9 bits in total
  unsigned int index = (addr >> 30) & 0x1FF;

  // find entry using index
  // if the present bit of attributes is 1, then it means that
  // the value in the entry points to next level table which is page directory
  // table
  if ((uint64_t)pdp[index] & PTE_P) {
    // clear attribute bits to get address
    // convert physical address to virtual
    pd = (PD)P2V(PDE_ADDR(pdp[index]));
  }
  // if the present bit is not set, then it is an unused entry
  // if alloc is 1, allocate a new page and set the entry to
  // make it point to this page
  else if (alloc == 1) {
    pd = (PD)kalloc();
    if (pd != NULL) {
      memset(pd, 0, PAGE_SIZE);
      pdp[index] = (PD)(V2P(pd) | attribute);
    }
  }

  return pd;
}

// map_pages mapps the address to physical page
// page is the base physical addr of the 2M page
bool map_pages(PML4 pml4, vaddr_t s, vaddr_t e, paddr_t page,
               uint32_t attribute) {
  // aligned virtual addresses
  vaddr_t vstart = PA_DOWN(s);
  vaddr_t vend = PA_UP(e);

  // checks start and end of region
  ASSERT(s < e);
  // make sure the physical address we want to map into is page aligned
  ASSERT(page % PAGE_SIZE == 0);
  // checks if end of physical address is outside the range of 1G memory
  ASSERT(page + vend - vstart <= 1024 * 1024 * 1024);

  do {
    PDP pdp = find_pml4t_entry(pml4, vstart, 1, attribute);
    if (pdp == NULL) return false;

    PD pd = find_pdp_entry(pdp, vstart, 1, attribute);
    if (pd == NULL) return false;

    // set pd entry which points to the 2M page

    // index value is 9 bits in total starting from bits 21
    // so we shift right 21 bits and clear other bits excep the lower 9 bits of
    // the result
    unsigned int index = (vstart >> 21) & 0x1FF;
    // if the present bit is set, it means that we
    // remap to the used page
    ASSERT(((uint64_t)pd[index] & PTE_P) == 0);

    // set entry
    // PTE_ENTRY indicates that this is 2M page
    pd[index] = page | attribute | PTE_ENTRY;

    // move to the next page
    vstart += PAGE_SIZE;
    page += PAGE_SIZE;

    // if the virtual address is still within the memory region,
    // we continue the process until we reach end of region
  } while (vstart + PAGE_SIZE <= vend);

  return true;
}

// switch_vm loads cr3 register with new PML4 table to make the mapping
void switch_vm(PML4 pml4) { load_cr3(V2P(pml4)); }

// each process has its own address space,
// any changes in one address space will not affect other address spaces
// so we can setup kernel space and user space for each process

// setup_kvm remaps the kernel using 2MB pages and returns the new PML4 table
PML4 setup_kvm(void) {
  PML4 pml4 = (vaddr_t)kalloc();
  if (pml4 != 0) {
    memset((void *)pml4, 0, PAGE_SIZE);
    // KERNEL_BASE is start of memory region
    // PTE_P | PTE_W specify that the kernel memory is readable, writable
    // and not accessible by user applications
    if (!map_pages(pml4, KERNEL_BASE, mem_end, V2P(KERNEL_BASE),
                   PTE_P | PTE_W)) {
      free_uvm(pml4);
      pml4 = 0;
    }
  }

  return pml4;
}

void init_kvm(void) {
  PML4 pml4 = setup_kvm();
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
void free_pages(PML4 pml4, vaddr_t start, vaddr_t end) {
  // start and end have to be aligned
  ASSERT(start % PAGE_SIZE == 0);
  ASSERT(end % PAGE_SIZE == 0);

  do {
    // find the page directory table
    // alloc is 0 which means
    // find_pdp_entry returns null if the entry does not exist
    // because we want to free the existing page
    PD pd = find_pdp_entry(pml4, start, 0, 0);

    if (pd != NULL) {
      // index is used to locate entry in the page directory table
      unsigned int index = (start >> 21) & 0x1FF;
      // find corresponding entry and check present bit
      if (pd[index] & PTE_P) {
        // the lower 21 bits should be cleared before using the address.
        // so PTE_ADDR clears the lower 21 bits
        kfree(P2V(PTE_ADDR(pd[index])));
        // the entry now is unused
        pd[index] = 0;
      }
    }

    // move to next page
    start += PAGE_SIZE;
    // continue process until we reach end of memory region
  } while (start + PAGE_SIZE <= end);
}

// loop through the PML4 table and page directory pinter tables
// to find page directory tables.
static void free_pd(PML4 pml4) {
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
static void free_pdp(PML4 pml4) {
  for (int i = 0; i < 512; i++) {
    // if the entry is used, it frees the page pointed to by the entry
    // which is page directory pointer table
    if ((uint64_t)pml4[i] & PTE_P) {
      kfree(P2V(PDE_ADDR(pml4[i])));
      pml4[i] = 0;
    }
  }
}

static void free_pml4t(PML4 pml4) { kfree(pml4); }

// to free vm we free the physical pages in the user space as
// well as page tables.
// because we need the page tables to locate the physical pages
// we first free the pages by calling free_pages
// then we free the page directory tables
// and page directory pointer tables.
// we do not free the kernel page
// because the kernel page is shared among all the vms
void free_uvm(PML4 pml4) {
  free_pages(pml4, 0x400000, 0x400000 + PAGE_SIZE);
  free_pd(pml4);
  free_pdp(pml4);
  free_pml4t(pml4);
}

// we map only one page for the user programs which means
// the code, data and stack of the programs are located in the same 2M page.

// size is the size of the program we will copy
bool setup_uvm(PML4 pml4, paddr_t program, int size) {
  // allocate a page which is used to
  // store the code, data and stack of the program
  void *page = kalloc();

  bool status = false;

  if (page != NULL) {
    // it could include some random values
    memset(page, 0, PAGE_SIZE);
    // map the virtaul address (0x400000) to the physical 2M page
    // because we implement only one page for user application,
    // we add page size to the base address
    // and next one is the base of physical page we want to map into
    // which is the page we allocate
    status = map_pages(pml4, 0x400000, 0x400000 + PAGE_SIZE, V2P(page),
                       PTE_P | PTE_W | PTE_U);
    if (status == true) {
      memcpy(page, (void *)program, size);
    } else {
      kfree((vaddr_t)page);
      free_uvm(pml4);
    }
  }

  return status;
}