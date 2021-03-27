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
#include "../util/reportError.h"
#include "../device/uart.h"

/* Set and Enviornment variable so that malloc can recognize that the memory requested is for a file/file_node so, it assigns the default PID */
bool MALLOC_FOR_OS_ENV = 0;

/* Function myMalloc */
void *SVCmyMallocImpl(unsigned int requestedSize)
{
	/* We need metadata block pointers to traverse through the mainList.*/
	struct block *current;
	void *result;
	unsigned int noOfBytes;

	/* Disable interrupts */
	di();

	/* If a request is made to allocate 0 bytes of memory,
	 * the function should return a NULL pointer.  The
	 * allocated memory is not to be initialised.*/
	if(requestedSize == 0)
		{
			ei();
			return(NULL);
		}

	/* round up the number of Bytes to nearest double word boundary */
	noOfBytes = roundUp(requestedSize);

	/* This result pointer is to return the starting address
	 * of the allocated chunk of memory.*/
	if(NULL == mainList)
	{   /* Initializing the block of memory */
		initialize();
	}

	/* This condition will initialise the memory if it is not initialized at the
	 * beginning. This set of statements will be executed only if the size of the
	 * first metadata block is not set yet. That means, if the memory is still not
	 * initialized.*/
	current=mainList;

	/* Now we make the temporary pointer current to point to the start of the metadata
	 * block list.*/

	while(current)
	{
		/* If you have the exact sized block and isFree (Do not travase) */
		if(((current->size)==noOfBytes) && (current->isFree == true))
			break;

		/* If block has higher than or equal to minimum required size.*/
		if((current->size)>=(noOfBytes+sizeof(struct block)))
		{

			/* Following means creation of zero block which is not allowed, hence move forward*/
			if((current->size)==(noOfBytes+sizeof(struct block)))
				goto NEXT;

			/* if size is ok, and if the block is free then break (Do not travase through list) */
			if(current->isFree == true)
				break;
		}

		NEXT:

		/* If this condition is met, the metadata block we checked cannot be used
		 * for the allocation. So we execute the following statements. That is
		 * you go on checking one metadata block at a time.*/
		/* previous=current; */
		current=current->next;
	}

	if(NULL == current) /* That means you traversed through the entire list but you did not find the right block */
	{
		/* Immidiatly Exit */
		goto EXIT;
	}

	/* Now we are ensured that current is not NULL */
	if((current->size)==noOfBytes)
	{
		/* If this condition is met, the metadata block we checked refers to
		 * a chunk of memory that exactly fits the required size. So set the
		 * free flag to 0, indicating that it is allocated. Then return the
		 * starting address of the block of allocated memory.*/
		current->isFree = false;
		current->size = noOfBytes;
		if(MALLOC_FOR_OS_ENV)
			current->pid = OCCUPIED_PID_BY_OS_DEFAULT;
		else
			current->pid  = SVCgetCurrentPIDImpl();
		result=(current->data);		
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
	}
	else
	{
		/* Else it means that there is no sufficient memory to allocate so we
		 * should return a NULL pointer.*/
		result=NULL;
	}

	EXIT:

	/* Enable Interrupts */
	ei();
	return result;
}


/* Making way for a new block allocation by splitting a free block
 * (Using first fit algorithm)*/
void split(struct block *fitting_slot,uint32_t size)
{
	struct block *new;
	new = (struct block *)(fitting_slot->data + size);
	new->size = (fitting_slot->size) - size - sizeof(struct block);
	new->isFree = true;
	new->pid = FREE_PID_DEFAULT;
	new->next = (fitting_slot->next);
	(fitting_slot->size)= size;
	(fitting_slot->isFree)= false;
	(fitting_slot->next)= new;

	if(MALLOC_FOR_OS_ENV)
		(fitting_slot->pid) = OCCUPIED_PID_BY_OS_DEFAULT;
	else
		(fitting_slot->pid) = SVCgetCurrentPIDImpl();
}


/* This is to merge the consecutive free blocks by
 * removing the metadata block in the middle. */
void merge(struct block *current,struct block *previous)
{
	/* merge to consequtive free blocks*/
	if((current->isFree) && (current->next->isFree))
	{
		current->size = current->size + (current->next->size) + sizeof(struct block);
		current->next = (current->next->next);
	}

	/* if previous was free and current is also free then combine them to reserve to the original size*/
	if(previous)
	{
		if((previous->isFree) && (current->isFree))
		{
			previous->size = current->size + (previous->size) + sizeof(struct block);
			previous->next = current->next;
		}
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

/* Function myFreeErrorCode(free) */
int SVCmyFreeErrorCodeImpl(void* ptr)
{
	struct block *current,*previous = NULL;
	current = mainList;
	bool match = false;
	pid_t currentPIDtoFree;

	if(MALLOC_FOR_OS_ENV)
		currentPIDtoFree = OCCUPIED_PID_BY_OS_DEFAULT;
	else
		currentPIDtoFree = SVCgetCurrentPIDImpl();

	/* Disable interrupts */
	di();

	/* If the value of the parameter is NULL, the call has no effect. */
	if (NULL == ptr)
	{
		ei();
		return NULL_POINTER_NO_EFFECT;
	}

	/* If the "ptr" parameter does not point to a region in the memory,
	 * the call should have no effect. Here we check whether the address
	 * to which the pointer given as an argument to the function actually
	 * lies within the address range of the main memory. */
	if((ptr<head)||((long int *)ptr>=((long int *)head+MAX_MEMORY_SIZE)))
	{
		ei();
		return OUT_OF_MAIN_MEMORY;
	}

	/* Perform Second level check to make sure that the pointer is a valid
	 * address within the memory.*/
	while(current)
	{
		/* Do not allow other process to free the memory of different process except the 
		 * shell is allowed to free other processes memories */
		if(ptr == current->data)
		{
			/* Either process itself or the shell can kill the memory occupied by the process */
			if((currentPIDtoFree == current->pid) || (currentPIDtoFree == OCCUPIED_PID_BY_OS_DEFAULT))
			{
				match = true;
				break;
			}

		}

		previous = current;
		current=current->next;
	}

	/* If match is not equal to true then the address specified was not valid address to be freed */
	if (match != true)
	{
		ei();
		return INVALID_ADDRESS_IN_THE_MEMORY;
	}

	/* If there is a match then, we simply set the free flag in the metadata
	 * block to 1 indicating that it is free and scan through and merge the
	 * consecutive blocks that are free, if any.*/
	freeAndMergeAction(current,previous);

	/* Enable Interrupts */
	ei();

	return MEMORY_FREE_SUCCESS;
}

void SVCmyFreeImpl(void *pointer)
{
	SVCmyFreeErrorCodeImpl(pointer);
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


void SVCmyMemoryMapImpl(void)
{
	struct block *current;
	current = mainList;
	char buffer[100];
	int len=0;

	/* Disable interrupts */	

	SVCfputs_mainImpl(STDOUT,"-----------------------------------\r\n");
	SVCfputs_mainImpl(STDOUT,"     Main Memory Starts      \r\n");
	len = snprintf(buffer,100,"          Head: %p         \r\n",head);
	if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);
	len = snprintf(buffer,100," Metadata size: %lu Bytes    \r\n",(long unsigned int)sizeof(struct block));
	if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

	while(1)
	{
		SVCfputs_mainImpl(STDOUT,"-----------------------------------\r\n");

		len = snprintf(buffer,100," PID          = %lu \r\n",(unsigned long)current->pid);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

		len = snprintf(buffer,100," isFree       = %d \r\n",current->isFree);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

		len = snprintf(buffer,100," Size         = %lu Bytes \r\n",(unsigned long)current->size);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

		len = snprintf(buffer,100," Data Address = %p \r\n",current->data);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

		len = snprintf(buffer,100," Next Block   = %p \r\n",(void*)current->next);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);

		/* Print the list until you hit the end of the list */
		if(NULL != current->next)
			current = current->next;
		else
		{
			SVCfputs_mainImpl(STDOUT,"-----------------------------------\r\n");
			SVCfputs_mainImpl(STDOUT,"     Main Memory Ends     \r\n");
			SVCfputs_mainImpl(STDOUT,"-----------------------------------\r\n");
			break;
		}        
	}
}

void freeAndMergeAction(struct block *currentBlkToFree,struct block *previousBlk)
{
	currentBlkToFree->isFree=true;
	currentBlkToFree->pid=FREE_PID_DEFAULT;
	merge(currentBlkToFree,previousBlk);
}

int freeMemoryForPID(pid_t inputPID)
{
	struct block *current,*previous = NULL;
	current = mainList;

	/* Disable Interrupts */
	di();

	/* Traverse through list and free memory occupied by the PID */
	while(current)
	{
		if(current->pid == inputPID)
		{
			freeAndMergeAction(current,previous);
		}

		previous = current;
		current=current->next;
	}

	/* Enable Interrupts */
	ei();

	return(SUCCESS);
}

void setMallocFileSystemEnv()
{
	MALLOC_FOR_OS_ENV = true;
}

void unsetMallocFileSystemEnv()
{
	MALLOC_FOR_OS_ENV = false;
}

void setMallocForOSEnv()
{
	MALLOC_FOR_OS_ENV = true;
}

void unsetMallocForOSEnv()
{
	MALLOC_FOR_OS_ENV = false;
}

bool queryMALLOCForOSEnv()
{
	return(MALLOC_FOR_OS_ENV);
}

uint64_t getMemoryOccupiedByPID(pid_t pid)
{
	uint64_t tempSize = 0;
	struct block *current = mainList;

	while(current)
	{
		if(current->pid==pid && current->isFree==false)
		{
			tempSize += current->size;
		}

		current = current->next;
	}

	return(tempSize);
}
