/* This source file is a implementation of memory allocation and deallocation.
 *
 * The myMalloc function is declared as taking an unsigned int
 * as its only parameter and returning a pointer to void
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 */

#include <stdio.h>
#include "memory.h"
#include "reportError.h"    

/* Function myMalloc */
void *myMalloc(unsigned int requestedSize)
{
    /* We need metadata block pointers to traverse through the freeList.*/
    struct block *current;
    void *result;
    unsigned int noOfBytes;
    
    /* If a request is made to allocate 0 bytes of memory,
     * the function should return a NULL pointer.  The
     * allocated memory is not to be initialized.*/
    if(requestedSize == 0)
        return(NULL);
    
    /* round up the number of Bytes to nearest double word boundry */
    noOfBytes = roundUp(requestedSize);
    
    /* This result pointer is to return the starting address
     * of the allocated chunk of memory.*/
    if(NULL == freeList)
    {   /* Initializing the block of memory */
        initialize();
    }
    
    /* This condition will initialize the memory if it is not initialized at the
     * beginning. This set of statements will be executed only if the size of the
     * first metadata block is not set yet. That means, if the memory is still not
     * initialized.*/
    current=freeList;
    
    /* Now we make the temporary pointer current to point to the start of the metadata
     * block list.*/
	
    while(1) 
    {
    	/* If you have the exact sized block (Do not travase) */
    	if(((current->size)==noOfBytes) && (current->free == 1))
    		break;
    	
    	/* If block has higher than minimum required size.*/
    	if((current->size)>=(noOfBytes+sizeof(struct block))) 	
		{				  	
			
			/* Following means creation of zero block which is not allowed, hence move forward*/
			if((current->size)==(noOfBytes+sizeof(struct block)))
				goto NEXT;
	    	
			/* if size is ok, and if the block is free then break (Do not travase through list) */
	    	if(current->free == 1)
	    		break;					
		}
		
		NEXT:
		/* if you are at the end of the list break. */
		if(NULL==(current->next))
			break;
				
        /* If this condition is met, the metadata block we checked cannot be used
         * for the allocation. So we execute the following statements. That is
         * you go on checking one metadata block at a time.*/
        /* previous=current; */ 	   
		current=current->next;
    }
    
    if((current->size)==noOfBytes)
    {
        /* If this condition is met, the metadata block we checked refers to
         * a chunk of memory that exactly fits the required size. So set the
         * free flag to 0, indicating that it is allocated. Then return the
         * starting address of the block of allocated memory.*/
        current->free=0;
        current->size = noOfBytes;
        current->pid = getCurrentPID();
        result=(current->data);
        return result;
    }
    
    else if ((current->size)>(noOfBytes+sizeof(struct block)))
    {
        /* If this condition is met, the metadata block we checked
         * refers to a chunk of memory that is of size greater than or exact equal to what
         * is required. So invoke the split() function to allocate only the
         * block which is required and then return the starting address of the
         * block of allocated memory.*/
        split(current,noOfBytes);
        result=(void*)(current->data);
        return result;
    }
    
    else
    {
        /* Else it means that there is no sufficient memory to allocate so we
         * should return a NULL pointer.*/
        result=NULL;        
        return result;
    }
}


/* Making way for a new block allocation by splitting a free block
 * (Using first fit algorithm)*/
void split(struct block *fitting_slot,uint32_t size)
{
    struct block *new;
    new = (struct block *)(fitting_slot->data + size);
 	new->size = (fitting_slot->size) - size - sizeof(struct block);
    new->free = 1;
    new->pid = 0xFF;
    new->next = (fitting_slot->next);
    (fitting_slot->size)= size;
    (fitting_slot->free)=0;
    (fitting_slot->next)= new;
    (fitting_slot->pid) = getCurrentPID();
}



/* This is to merge the consecutive free blocks by
 * removing the metadata block in the middle. */
void merge()
{
    struct block *current,*previous;
    current=freeList;
    
    while(NULL!=(current->next))
    {
        /* merge to consequtive free blocks*/
        if((current->free) && (current->next->free))
        {
            current->size = current->size + (current->next->size) + sizeof(struct block);
            current->next = (current->next->next);
        }
        previous=current;
        if(NULL!=(current->next))
            current=current->next;
    }
    
    /* if previous was free and current(last) is also free then combine them to reserve to the original size*/
    if((previous->free) && (current->free))
    {
        previous->size = current->size + (previous->size) + sizeof(struct block);
        previous->next = NULL;
    }
}


/*
 * The myFree function is declared as taking a pointer to void as its
 * only parameter and not returning anything
 *
 * The "ptr" parameter is a pointer to a region of memory previously
 * allocated by the myMalloc function.  The myFree function deallocates
 * the entire region of memory pointed to by the parameter.
 */

/* Function myFree(free) */
int myFreeErrorCode(void* ptr)
{
    struct block *current;
    current = freeList;
    bool match;
    
    match = false;
    
    /* If the value of the parameter is NULL, the call has no effect. */
    if (NULL == ptr)
        return NULL_POINTER_NO_EFFECT;
    
    /* If the "ptr" parameter does not point to a region in the memory,
     * the call should have no effect. Here we check whether the address
     * to which the pointer given as an argument to the function actually
     * lies within the address range of the main memory. */
    if((ptr<head)||((long int *)ptr>=((long int *)head+MAX_MEMORY_SIZE)))
        return OUT_OF_MAIN_MEMORY;
    
    /* Perform Second level check to make sure that the pointer is a valid
     * address within the memory.*/
    while(NULL!=(current->next))
    {
        if(ptr == current->data)
        {
            match = true;
            break;
        }
        
        if(NULL!=(current->next))
            current=current->next;
    }
    
    /* If match is not equal to true then the address specified was not valid address to be freed */
    if (match != true)
        return INVALID_ADDRESS_IN_THE_MEMORY;
    
    /* If there is a match then, we simply set the free flag in the metadata
     * block to 1 indicating that it is free and scan through and merge the
     * consecutive blocks that are free, if any.*/
    current=ptr;
    --current;
    current->free=1;
    current->pid=0xFF;
    merge();
    return MEMORY_FREE_SUCCESS;
}

void myFree(void *pointer)
{
	myFreeErrorCode(pointer);
	/* Ignore the returnStatus */
}


unsigned int roundUp(unsigned int n)
{
    /* Round up the number n to the nearest double word boundry i.e. multiple = 4*2 */
    const int multiple = 8;
    
    /* The pointer returned by myMalloc should always point to a region of
     * memory that starts on an 8-byte (double-word) boundary, round-up the
     * amount of memory requested by the user so that the alignment requirement is satisfied.*/
    return (n + multiple - 1) & ~(multiple - 1);
}



void myMemoryMap(void)
{
    struct block *current;
    current = freeList;
    fprintf(stdout,"-----------------------------------\n");
    fprintf(stdout,"     Main Memory Starts      \n");
    fprintf(stdout,"          Head: %p         \n",head);
    fprintf(stdout," Metadata size: %lu Bytes    \n",(long unsigned int)sizeof(struct block));
    
    while(1)
    {
        fprintf(stdout,"-----------------------------------\n");
        fprintf(stdout," PID          = %d \n" , current->pid);
        fprintf(stdout," isFree       = %d \n", current->free);
        fprintf(stdout," Size         = %d Bytes \n", current->size);
        fprintf(stdout," Data Address = %p \n",current->data);
        fprintf(stdout," Next Block   = %p \n",(void*)current->next);
        
        /* Print the list until you hit the end of the list */
        if(NULL != current->next)
            current = current->next;
        else
        {
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout,"     Main Memory Ends     \n");
            fprintf(stdout,"-----------------------------------\n");
            break;
        }
        
    }
}
