#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../util/reportError.h"
#include "../util/util.h"
#include "sdram.h"
#include "../device/lcdc.h"
#include "../device/deviceEnums.h"
#include "../device/deviceFcnDeclaration.h"
#include "../process/processControlUtils.h"
#include "../int/interrupt.h"

/* FREE_PID_DEFAULT means this memory can be used by any process or OS itself */
#define FREE_PID_DEFAULT 0

/* OCCUPIED_PID_BY_OS_DEFAULT means that this memory is being used by the process 1 which is a shell */
#define OCCUPIED_PID_BY_OS_DEFAULT 1

#define MAX_MEMORY_SIZE (SDRAM_SIZE - (LCDC_GW_START_ADDRESS + LCDC_GW_BUFFER_SIZE))

/*The structure definition to contain metadata of each block allocated or deallocated*/
struct block{
	uint32_t size; /*Carries the size of the block described by it*/
    pid_t pid;
    uint8_t isFree;  /*This flag is used to know whether the block described by the metadata structure is free or not -- > if free, it is set to 1. Otherwise 0.*/
	struct block *next; /*Points to the next metadata block size */
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
struct block *mainList;
/* NOTE: In future there will be an address to the global memory for K70 */

/* Function deceleration */
int initializeMainMemory(void);
int freeMainMemory(void);
void *SVCmyMallocImpl(unsigned int);
void SVCmyFreeImpl(void *);
int SVCmyFreeErrorCodeImpl(void *);
void SVCmyMemoryMapImpl(void);
void initialize(void);
void split(struct block *,uint32_t);
void merge(struct block *,struct block *);
pid_t getCurrentPID(void);
unsigned int roundUp(unsigned int);
int freeMemoryForPID(pid_t);
void freeAndMergeAction(struct block *,struct block *);
void setMallocFileSystemEnv(void);
void unsetMallocFileSystemEnv(void);
void setMallocForOSEnv(void);
void unsetMallocForOSEnv(void);
bool queryMALLOCForOSEnv(void);

#endif /* ifndef _MEM_H */
