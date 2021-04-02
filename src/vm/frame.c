/* Might not need all and might need more*/
#include "vm/frame.h"
#include <bitmap.h>
#include <debug.h>
#include <list.h>
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "userprog/pagedir.h"
#include "vm/swap.h"

/*Lock for the frame table*/
static struct lock ftable_lock;

static struct list ftable;


/*Initialize the frame table*/
void
ftable_init(void)
{
    lock_init(&ftable_lock);
    list_init(ftable);
}

/*Allocates the frame table entry setting its address ....*/
void *
frame_allocate(enum palloc_flags)
{

}

/*frees the frame*/
void frame_free(struct frame *f)
{

}

