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
    //Initialize frame parameters, list and lock.
    lock_init(&ftable_lock);
    list_init(&ftable);
}

/*Allocates the frame table entry setting its address ....*/
void *
frame_allocate(enum palloc_flags flags)
{
    ASSERT(flags & PAL_USER);

    lock_acquire(&ftable_lock);

    //Allocate the page, passing in the flags, of which PAL_USER should be set.
    void *kpage = palloc_get_page(flags);
    //If we get back a null page, release lock and return.
    if(kpage == NULL)
    {
        lock_release(&ftable_lock);
        return NULL;
    }
    //Allocate some memory for the frame struct.
    struct frame *f = malloc(sizeof(struct frame));
    if(f == NULL)
    {
        //If our allocation goes bad, 
        palloc_free_page(kpage);
        return NULL;
    }
    //Associate the frame with the page we allocated earlier.
    f->page = kpage;
    //Put in the frame table.
    list_push_back(&ftable, &f->elem);
    //Unlock
    lock_release(&ftable_lock);
    return kpage;
}

/*frees the frame*/
void frame_free(struct frame *f)
{
    //Grab the lock for frame ops.
    lock_acquire(&ftable_lock);

    struct frame *frame = NULL;
    struct list_elem *e;
    for(e = list_begin(&ftable); e != list_end(&ftable); e = list_next(e))
    {
        frame = list_entry(e, struct frame, elem);
        if(frame->page == f->page)
        {
            f = frame;
            break;
        }
    }

    if(f != NULL)
    {
        list_remove(&f->elem);
        free(f);
    }
    palloc_free_page(f->page);

    lock_release(&ftable_lock);
}

/* Removes a frame but does not free from memory, may be useful later for swapping. */
void frame_remove(struct frame *f)
{
    struct list_elem *e;
    struct frame *frame;
    //Critical section
    lock_acquire(&ftable_lock);
    //Find the given frame.
    for(e = list_begin(&ftable); e != list_end(&ftable); e = list_next(e))
    {
        frame = list_entry(e, struct frame, elem);
        if(frame->page == f->page)
        {
            f = frame;
            break;
        }
    }
    //If we find the given frame in the frame table, remove it from the list and free the STRUCT but not the PAGE.
    if(f != NULL)
    {
        list_remove(&f->elem);
        free(f);
    }
    lock_release(&ftable_lock);
}