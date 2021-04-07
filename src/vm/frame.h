#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "threads/palloc.h"
#include "vm/page.h"

/* PROJECT 3 */

struct suppl_pt *suppl_pt_create (void);
void suppl_pt_destroy (struct suppl_pt *);

bool suppl_pt_set_zero (void *upage);
bool suppl_pt_set_file (void *upage, struct file *, off_t, uint32_t read_bytes,
                        uint32_t zero_bytes, bool writable);
bool suppl_pt_load_page (void *upage);
void suppl_pt_clear_page (void *upage);

/*Frame table entry. The frame table is going to be a list*/
struct frame
{
    void * page; 				/* Physical address of kernel page. */
    struct list_elem elem;      /* Structured as a list */
};

void ftable_init(void); //initialization of the frame table 
void * frame_allocate(enum palloc_flags); //allocates the frame table entry
void frame_free(struct frame *); //frees the frame table entry from the frame

#endif

