/*
 * getCurserLocation function helps to identify the next blank space in the sentence.
 *
 * If the previous curser location is provided, it will count untill the next
 * location to search for the space, and return that as next location.
 *
 * if loop control reaches at the end of the string then it breaks the loop and sends the
 * count to the calling program.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */
#include <stdlib.h>

int getCurserLocation(int pre,char *string)
{
    int i;
    
    for(i=pre; string[i] != ' '; i++)
    { /* Do nothing, Just count until the next space */
        if(string[i] == '\0')
            break;
    }
    return(i);
}
