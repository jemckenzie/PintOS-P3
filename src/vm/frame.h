#ifndef VM_FRAME_H
#define VM_FRAME_H

/*Might need more includes !!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#include <list.h>
#include "threads/palloc.h"
#include "vm/page.h"

//Function definitions for includes
struct suppl_pt *suppl_pt_create (void);
void suppl_pt_destroy (struct suppl_pt *);

bool suppl_pt_set_zero (void *upage);
bool suppl_pt_set_file (void *upage, struct file *, off_t, uint32_t read_bytes,
                        uint32_t zero_bytes, bool writable);
bool suppl_pt_load_page (void *upage);
void suppl_pt_clear_page (void *upage);

/*Frame table entry. The framme table is going to be a list*/
struct frame
{
    void *page; //Pointer to page. Every entry in frame table points to the page ("kernel page maps to frame", is what HangPark says)
    struct list_elem elem;      /* Structured as a list */
};

void ftable_init(void); //initialization of the frame table 
void * frame_allocate(enum palloc_flags); //allocates the frame table entry
void frame_free(struct frame *); //frees the frame table entry from the frame

#endif

