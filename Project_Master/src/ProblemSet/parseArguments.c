/*
 * This is a main parser which parses the input string in to argv array.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <stdlib.h>
#include "memory.h"

int getCurserLocation(int,char*);

char** parseArguments(int argc, char *args)
{
    /* First MALLOC for array of pointer pointing to a location */
    char **argv = (char**) myMalloc((argc+1)*sizeof(char*));
    
    int numArgs = 0;
    int i, j ,n ,previousCursor = 0, nextCursor = 0;
    
    if(!argv)
        return(NULL); /* MEMORY_ALLOCATION_FAILED */
        
    /* argv array should be terminated by NULL (Remember: 0 based indexing)*/
    argv[argc] = '\0';
    
    for(numArgs = 0; numArgs < argc ;numArgs++)
    {
        nextCursor = getCurserLocation(previousCursor, args);
        /* n = next - previous. length of space seperated fields */
        n = nextCursor - previousCursor;
        
        argv[numArgs] = (char*)myMalloc((n+1)*sizeof(char));
        
        if(NULL == argv[numArgs])
            return(NULL); /* MEMORY_ALLOCATION_FAILED */
        
        j = 0;
        for(i=previousCursor;i<previousCursor+n;i++)
        {
            argv[numArgs][j] = args[i];
            j = j + 1;
        }
        argv[numArgs][j] = '\0';
        
        /* +1 because we have created the buffer in such a way that only one space is available. */
        previousCursor = nextCursor + 1;
    }
    return(argv);
}
