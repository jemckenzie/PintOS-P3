#include "vm/page.h"
#include "vm/frame.h"
#include <list.h>
#include <string.h>
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"


/* Function that creates a supplemental page table for a process */
struct supplemental_pt * new_supplemental_table(void)
{
    //Allocate some mem
    struct supplemental_pt * supp_pt = malloc(sizeof(struct supplemental_pt));
    if(supp_pt == NULL)
    {
        return NULL;
    }
    list_init(&supp_pt->page_list);

    return supp_pt;
}

/* Destroys given supplemental page table.  Frees supp pt entries, not frames. */
void supp_pt_destroy(struct supplemental_pt *spt)
{
    struct list_elem *e;
    if(!list_empty(&spt->page_list))
    {
        for(e = list_begin(&spt->page_list); e != list_end(&spt->page_list); e = list_next(e))
        {
            struct spt_entry *spte = list_entry(e, struct spt_entry, elem);
            free_entry(spte);
        }
    }
    free(spt);
}

/* Locates a user page entry in the supplemental page table. */
struct spt_entry * locate_page(void *user_page)
{
    struct supplemental_pt *table = thread_current()->spt;
    struct list_elem *e;

    for(e = list_begin(&table->page_list); e != list_end(&table->page_list); e = list_next(e))
    {
        struct spt_entry *entry = list_entry(e, struct spt_entry, elem);
        //Located the 
        if(entry->user_page == user_page)
        {
            return entry;
        }
    }
    return NULL;
}

/* Frees an individual supplemental page table entry.  Needs implementation for swapping. */
void free_entry(struct spt_entry *spte)
{
    if(spte == NULL)
    return;

    if(spte->kernel_page != NULL)
    {
        frame_remove(spte->kernel_page);
    }
    else if(spte->pagetype == PAGE_SWAP)
    {
        //GOTTA FILL THIS IN STILL
    }
    list_remove(&spte->elem);
    free(spte);
}

/* Zeroed supplemental page table entry is allocated with passed in user page address */
bool suppl_pt_zero_allocate(void *user_page)
{
    struct spt_entry *spte = malloc(sizeof(struct spt_entry));
    if(spte == NULL)
    {
        return false;
    }

    spte->pagetype = PAGE_ZERO;
    spte->user_page = user_page;
    spte->kernel_page = NULL;
    spte->dirty = false;  //It was just allocated so it hasn't been dirtied by a write yet.

    struct supplemental_pt *pt = thread_current()->spt;
    //Add the new entry to the thread's supplemental page table.
    list_push_back(&pt->list, &spte->elem);
    return true;
}

/* Allocates a page for a given user page with the file parameters passed in. */
bool suppl_pt_file_allocate(void *user_page, struct file *file, off_t offset,
uint32_t read_bytes, uint32_t zero_bytes, bool writable)
{
    struct spt_entry *spte = malloc(sizeof(struct spt_entry));
    if(spte == NULL)
    return false;

    //It's a file page.
    spte->pagetype = PAGE_FILE;
    spte->user_page = user_page;
    spte->kernel_page = NULL;
    spte->dirty = false;
    spte->file = file;
    spte->file_offset = offset;
    spte->read_bytes = read_bytes;
    spte->zero_bytes = zero_bytes;
    spte->writable = writable;

    //Add the new file entry to the supplemental page table.
    struct supplemental_pt *pt = thread_current()->spt;
    list_push_back(&pt->page_list, &spte->elem);
    return true;
}

/* Loads a user page into a frame in physical memory. */
bool load_page(void *user_page)
{
    //Use the helper function to find the entry in the table.
    struct spt_entry *entry = locate_page(user_page);
    if(entry == NULL)
    {
        return false;
    }
    if(entry->kernel_page != NULL)
    {
        return false;
    }

    //Allocate a frame using the function we defined in frame.  Uses palloc_get_page with the USER_PAL flag set to grab from the user pool.
    void *kernel_page = frame_allocate(PAL_USER);
    //Error check
    if(kernel_page == NULL)
        return false;
    
    bool writable = true;
    switch(entry->pagetype)
    {
        case PAGE_ZERO:
        //Set it to all zeroes in memory for a zeroed page.
            memset(kernel_page, 0, PGSIZE);
            break;

        case PAGE_FILE:
            //Find the position in the file.
            file_seek(entry->file, entry->file_offset);
            //If we have an unequal amount of read bytes in page table entry to the file_read operation
            if(file_read(entry->file, kernel_page, entry->file_offset) != (int) entry->read_bytes)
            {
                //Free the frame and get out because something's wrong.
                frame_free(kernel_page);
                return false;
            }
            //Set up in memory some zeroes at the position of the file's offset with a value of the zero bytes field.
            memset(kernel_page + entry->read_bytes, 0, entry->zero_bytes);
            writable = entry->writable;
            break;

        case PAGE_SWAP:
        //Not yet implemented
            break;

        default:
        //Panic the kernel.
            NOT_REACHED();
    }

    uint32_t *pd = thread_current()->pagedir;
    //Check and see if we can set the user page to map to a frame in physical memory.
    //Error check
    if(!pagedir_set_page(pd, user_page, kernel_page, writable))
    {
        frame_free(kernel_page);
        return false;
    }
    //Set the pte's kernel page field to the one we allocated from the user page pool.
    entry->kernel_page = kernel_page;

    return true;
}

/* Clears the entry from the supplemental page table of the current thread.  Removes the table element, but doesn't free the frame.
Will cause the passed in user_page to page fault. */
void clear_page_from_table(void *user_page)
{
    //Find the page.
    struct spt_entry *entry = locate_page(user_page);
    pagedir_clear_page(thread_current()->pagedir, user_page)
    free_entry(entry);
}