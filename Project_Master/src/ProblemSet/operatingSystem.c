/*
 * This program is a Simple Operating System Implementation in C language
 * Course: Principles of Operating Systems
 * Harvard Extension School CSCI E-92
 *
 * This program includes main() function which allocates main memory and runs a shell.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "reportError.h"
#include "memory.h"
#include <assert.h>

int shell(void);

int main()
{
	/* Capture the errorCode */
    errorCode rootStatus;
        
	/* Allocate global memory : Initialize the main memory block with size 1 MB */
    rootStatus = initializeMainMemory();
    if(rootStatus!=SUCCESS) reportError(rootStatus);
    
    /* Call infinite While loop to run the shell continously */
    rootStatus = shell();
    
    /* Free Main memory of 1MB */
    freeMainMemory();    
    return(rootStatus);
}

