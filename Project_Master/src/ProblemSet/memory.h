#ifndef _MEM_H
#define _MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "reportError.h"
#include <stdbool.h>

/* Define size in byte 1000000 Byte = 1 MB */
#define MAX_MEMORY_SIZE 1000000

/*The structure definition to contain metadata of each block allocated or deallocated*/
struct block{
	uint32_t size; /*Carries the size of the block described by it*/
    uint8_t pid;
    uint8_t free;  /*This flag is used to know whether the block described by the metadata structure is free or not -- > if free, it is set to 1. Otherwise 0.*/
	struct block *next; /*Points to the next metadata block size = 8 bytes */
    uint8_t data[]; /* padding field */
};

/* sizeof(struct block) = 16
------------------------

------------------------
|data[0]|*next|free|pid|
------------------------
      	  size
------------------------
*/
    
/* Global Pointers pointing to memroy head (begining of the memory) and the block.*/
void *head;
struct block *freeList;
/* NOTE: In future there will be an address to the global memory for K70 */

/* Function decleration */
int initializeMainMemory(void);
int freeMainMemory(void);
void *myMalloc(unsigned int);
void myFree(void *);
int myFreeErrorCode(void *);
void myMemoryMap(void);
void initialize(void);
void split(struct block *,uint32_t);
void merge(void);
int getCurrentPID(void);
unsigned int roundUp(unsigned int);

#endif /* ifndef _MEM_H */
