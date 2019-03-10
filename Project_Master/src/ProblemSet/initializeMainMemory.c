#include "memory.h"

int initializeMainMemory()
{
    /* Allocate an appropriately sized region of memory and it should return a
     * pointer to (i.e., the address of) the first byte of that region */
    
    /* Allocate the max memory and return a blobal pointer to that memory */
    head = malloc(MAX_MEMORY_SIZE*sizeof(char));
    /* sizeof(char) == 1 byte, in this case 1000000*1 byte == 1 MB */
    
    /* If could not allocate the memory then return error code */
    if(!head)
        return(MEMORY_ALLOCATION_FAILED);
    
    /*Pointing to the main block of memory which is initially free (no storage allocation yet)*/
    freeList=head;
    
    /* Initializing the block of memory */
 	initialize();
    return(SUCCESS);
}


void initialize()
{
    /* Initializing the block of memory */
    freeList->size = MAX_MEMORY_SIZE-sizeof(struct block);
    freeList->free=1;
    freeList->pid=0xFF;
    freeList->next=NULL;
}

int freeMainMemory()
{
    free(head);
    return(SUCCESS);
}
