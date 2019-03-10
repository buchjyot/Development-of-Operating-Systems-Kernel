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
#include "reportError.h"
#include "memory.h"

int main()
{
    errorCode flag;
    char *ptr,*ptr1,*ptr2, *ptr3,*ptr4;
    
    /* Initialize the main memory block */
    flag = initializeMainMemory();
    if(flag!=SUCCESS)reportError(flag);
    fprintf(stdout,"%s \n", "Main Memory Map");
    myMemoryMap();
    
    fprintf(stdout,"\n%s \n", "===========================================================================");
    fprintf(stdout,"%s \n", "Test1 : Allocate the memory and show the memoryMap, Free the same memory");
    ptr = (char*) myMalloc(12*sizeof(char));
    myMemoryMap();
    myFree(ptr);
    myMemoryMap();    
    
    fprintf(stdout,"\n%s \n", "=================================================");
    fprintf(stdout,"%s \n", "Test2 : Allocate the memory twice and free them");
    ptr1 = (char*) myMalloc(12*sizeof(char));
    ptr2 = (char*) myMalloc(5*sizeof(char));
    myMemoryMap();
    myFree(ptr1);
    myMemoryMap();
    myFree(ptr2);
    myMemoryMap();
    
    fprintf(stdout,"\n%s \n", "===============================================================================================");
    fprintf(stdout,"%s \n", "Test3 : Allocate the memory thrice and try to free some other memory which is not a valid memory.");
    ptr1 = (char*) myMalloc(12*sizeof(char));
    ptr2 = (char*) myMalloc(5*sizeof(char));
    ptr3 = (char*) myMalloc(1024*sizeof(char));
    myMemoryMap();
    /* Do not allow following memory locations to be freed */
    myFree(((void *)((int*)head-100)));
    myFree(ptr1+18);
    myMemoryMap(); /* The memory map is same as the previous one */
    myFree(ptr1);
    myFree(ptr2);
    myFree(ptr3);
    myMemoryMap();
    
    fprintf(stdout,"\n%s \n", "==============================================");
    fprintf(stdout,"%s \n", "Test4 : Dont allow 2 free blocks back to back");
    ptr1 = (char*) myMalloc(12*sizeof(char));
    ptr2 = (char*) myMalloc(5*sizeof(char));
    ptr3 = (char*) myMalloc(18*sizeof(char));
    ptr4 = (char*) myMalloc(32*sizeof(char));
    myMemoryMap(); 
    myFree(ptr2);
    myFree(ptr3);
    myMemoryMap();
    myFree(ptr1);
    myFree(ptr4);
    myMemoryMap();
    
    freeMainMemory();
    return(SUCCESS);
}
