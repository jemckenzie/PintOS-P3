#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "threads/palloc.h"
#include "vm/page.h"

/* PROJECT 3 */

/*Frame table entry*/
struct frame
{
    void * page; 				/* Physical address of kernel page. */
    struct list_elem elem;      /* Structured as a list */
    struct spt_entry *spt_entry;
};

void ftable_init(void); //initialization of the frame table 
struct frame * frame_allocate(enum palloc_flags); //allocates the frame table entry
void frame_free(struct frame *); //frees the frame table entry from the frame
void frame_remove(void *);

#endif

