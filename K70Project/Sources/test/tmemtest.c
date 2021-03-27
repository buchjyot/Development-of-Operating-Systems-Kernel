/* This is a main test program to test the memory allocation and deallocation
 *
 * The program calls the myMalloc and myFree functions to allocate and deallocate the
 * memory.
 *
 * memoryMap function prints the memory map of the 1 MB data.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 */

#include <stdio.h>
#include <stdlib.h>
#include "../util/reportError.h"
#include "../mem/memory.h"
#include "../device/uart.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int tmemtest(int argc, char *argv[])
{
    
    /* Strictly accept no arguments */
    if(!(argc==1))
        return(INVALID_INPUT);
    
    char *ptr,*ptr1,*ptr2, *ptr3,*ptr4;
    errorCode status;
    svc_fputs_main(STDOUT,"Main Memory Map \r\n");
    svc_myMemoryMap();
    
    svc_fputs_main(STDOUT,"===========================================================================\r\n");
    svc_fputs_main(STDOUT,"Test1 : Allocate the memory and show the memoryMap, Free the same memory\r\n");
    ptr = (char*) svc_myMalloc(12*sizeof(char));
    svc_myMemoryMap();
    status = svc_myFreeErrorCode(ptr);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    
    svc_fputs_main(STDOUT,"===========================================================================\r\n");
    svc_fputs_main(STDOUT,"Test2 : Allocate the memory twice and free them \r\n");
    ptr1 = (char*) svc_myMalloc(12*sizeof(char));
    ptr2 = (char*) svc_myMalloc(5*sizeof(char));
    svc_myMemoryMap();
    status = svc_myFreeErrorCode(ptr1);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    status = svc_myFreeErrorCode(ptr2);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    
    svc_fputs_main(STDOUT,"===========================================================================\r\n");
    svc_fputs_main(STDOUT,"Test3 : Allocate the memory thrice and try to free some other memory which is not a valid memory.\r\n");
    ptr1 = (char*) svc_myMalloc(12*sizeof(char));
    ptr2 = (char*) svc_myMalloc(5*sizeof(char));
    ptr3 = (char*) svc_myMalloc(1024*sizeof(char));
    svc_myMemoryMap();
    /* Do not allow following memory locations to be freed */
    status = svc_myFreeErrorCode(((void *)((int*)head-100)));
    if(status!=OUT_OF_MAIN_MEMORY)return(status);
    status = svc_myFreeErrorCode((void *)((int*)ptr1+18));
    if(status!=INVALID_ADDRESS_IN_THE_MEMORY)return(status);
    svc_myMemoryMap(); /* The memory map is same as the previous one */
    status = svc_myFreeErrorCode(ptr1);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    status = svc_myFreeErrorCode(ptr2);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    status = svc_myFreeErrorCode(ptr3);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    
    svc_fputs_main(STDOUT,"===========================================================================\r\n");
    svc_fputs_main(STDOUT,"Test4 : Dont allow 2 free blocks back to back \r\n");
    ptr1 = (char*) svc_myMalloc(12*sizeof(char));
    ptr2 = (char*) svc_myMalloc(5*sizeof(char));
    ptr3 = (char*) svc_myMalloc(18*sizeof(char));
    ptr4 = (char*) svc_myMalloc(32*sizeof(char));
    svc_myMemoryMap();
    status = svc_myFreeErrorCode(ptr2);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    status = svc_myFreeErrorCode(ptr3);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    status = svc_myFreeErrorCode(ptr1);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    status = svc_myFreeErrorCode(ptr4);
    if(status!=MEMORY_FREE_SUCCESS)return(status);
    svc_myMemoryMap();
    
    return(SUCCESS);
}
