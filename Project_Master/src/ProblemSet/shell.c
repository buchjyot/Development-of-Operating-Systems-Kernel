/*
 * This program is a Simple Shell Implementation in C language
 * Course: Principles of Operating Systems
 * Harvard Extension School CSCI E-92
 *
 * This program includes shell() function which runs a shell and
 * accepts the argument passed as a string in to a char[256] sized
 * array and then parses the string in terms of argc and argv.
 *
 * It executes the command specified in the string and shows appropriate
 * output to the user. If proper number of arguments are not passed, it throws
 * error to the console.
 *
 * At the end of the program it frees all the allocated memory for argv using
 * freeAllocatedMemory. While(1) loop makes the shell to run continously and
 * interact with user.
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

void displayPrompt();
int readInputString(char *);
char** parseArguments(int, char*);
void freeparsedargs(char**);
int freeAllocatedMemory(int,char**);
errorCode executeCommand(int,char**);
int strcmp_fcn(char *, char *);

int shell()
{
	/* Capture the errorCode */
    errorCode status;
           
    /* Infinite While loop to run the shell continously */
    while(1)
    {        
        /* Count of the number of white space separated fields found in the input line */
        int argc;
        
        /* Character pointer pointing a character array for argv[] array */
        char **argv;
        
        /* Allocate the 256 size of a character buffer as input */
        char *input = (char*) myMalloc(256*sizeof(char));
        
        if(input == NULL)
        {
		   reportError(MEMORY_ALLOCATION_FAILED);
		   assert(argv != NULL);
		}
        
        displayPrompt();
        
        /* Reads input string and counts the space delimted fields. */
        argc = readInputString(input);
        
        /* Parse Arguments and return the argv array */
        argv = parseArguments(argc,input);
        
        /* if could not allocate the memory then report error */
        if(argv == NULL)
        {
		   reportError(MEMORY_ALLOCATION_FAILED);
		   assert(argv != NULL);
		}
		
        /* Execute the specific command requested by user and display status */
        status = executeCommand(argc,argv);
        
        if(argc>0)
            reportError(status);
        
        /* Free all allocated memory */
        myFree(input);
        freeAllocatedMemory(argc,argv);        
    }
       
    return(SUCCESS);
}

