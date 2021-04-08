#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <list.h>
#include "filesys/file.h"

/* Denotes the status of an individual page */
enum page_status
{
    PAGE_ZERO,
    PAGE_FILE,
    PAGE_SWAP
};

/*  Supplemental page table */
struct supplemental_pt
{
    struct list page_list;   /* List of pages */
};


struct spt_entry
{
    enum page_status pagetype;    /* the type of page, denoted by the page_status enum */
    void *user_page;              /* User page */
    void *kernel_page;            /* Represents a kernel page */

    bool dirty;                    /* Indicates if page has been "dirtied" */
    struct file *file;             /* File page contains */
    off_t file_offset;             /* Offset within the file */
    uint32_t read_bytes;           /* Bytes that have been read */
    uint32_t zero_bytes;           /* Zeroed bytes */
    bool writable;                 /* Flag that says page is writable */

    /* USE IF SWAPPING PAGE */
    unsigned swap_index;           /* Swap disk index for swap device */
    struct list_elem elem;         /* Element within the list(the supplemental page table itself) */
};

void supp_pt_destroy(struct supplemental_pt *);
struct spt_entry * locate_page(void *);
void free_entry(struct spt_entry*);
bool suppl_pt_zero_allocate(void *);
bool suppl_pt_file_allocate(void *, struct file *, off_t, uint32_t, uint32_t, bool);
bool load_page(void *);
void clear_page_from_table(void *);
#endif