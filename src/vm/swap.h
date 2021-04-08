#ifndef VM_SWAP_H
#define VM_SWAP_H

void swap_table_init(void);
void swap_remove(size_t);
bool swap_in(void *, size_t);
size_t swap_out(void *);

#endif