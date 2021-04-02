#ifndef VM_FRAME_H
#define VM_FRAME_H

/*Might need more includes !!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#include <list.h>
#include "threads/palloc.h"
#include "vm/page.h"

/*Frame table entry. The framme table is going to be a list*/
struct frame
{
    void *page; //Pointer to page. Every entry in frame table points to the page ("kernel page maps to frame", is what HangPark says)
};

void ftable_init(void); //initialization of the frame table 
void * frame_allocate(enum palloc_flags); //allocates the frame table entry
void frame_free(struct frame *); //frees the frame table entry from the frame

