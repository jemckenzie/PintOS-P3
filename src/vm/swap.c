#include <bitmap.h>
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "vm/swap.h"


/* Number of sectors per page. */
#define PAGE_SECTORS (PGSIZE / BLOCK_SECTOR_SIZE)

//swap table lock
static struct lock stable_lock;

//static struct bitmap *swap_table;


void 
swap_table_init(void)
{

}

void 
swap_remove(size_t size)
{

}

bool 
swap_in(void *kpage, size_t size)
{

}

size_t 
swap_out(void *kpage)
{

}