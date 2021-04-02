/* Denotes the status of an individual page */
enum page_status
{
    PAGE_ZERO,
    PAGE_FILE,
    PAGE_SWAP
};

/*  Supplemental page table */
struct supplemental_pagetable
{
    struct list list;   /* List of pages */
};