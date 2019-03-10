/*
 * The function executeCommand is the API for executing the command
 * and returning the status to main function for further proessing.
 *
 * This function takes care of calling the right function. If command specified
 * do not exist in the database then it returns INVALID_COMMAND errorcode.
 *
 * Hence, this is a funciton which bridges the gap between main and the core
 * commands for the shell.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "commandsDefination.h"
#include "reportError.h"

errorCode executeCommand(int numberOfArgs,char **argv)
{
    int commandIndex;
    errorCode val;
    commandIndex = stringMatch(argv);
    
    /* if command was not found then return the same output */
    if(commandIndex == INVALID_COMMAND)
        return(INVALID_COMMAND);
    
    /* Execute the command */
    val = commands[commandIndex].functionp(numberOfArgs,argv);
    
    /* Transfer return value to main function (calling function)*/
    return(val);
}

int stringMatch(char *argv[])
{
    int i,j,length=0;
    
    /* Compute the length of the first argument */
    for(i = 0; argv[0][i] != '\0'; i++)
        length = i+1; /*same as length = length + 1*/
    
    /* Loop through all the commands in the database. */
    for(i = 0; i < NUMBER_OF_COMMANDS ;i++)
    {
        /* Compare untill either argument ends or the name of command ends */
        for(j = 0; (j < length) && (commands[i].name[j] != '\0') ;j++)
        {
            if(commands[i].name[j] != argv[0][j])
                break;
        }
        
        /* if j == length then it has compared successfully the entire command and match was found. */
        if(j==length)
            return(i); /* Send the command index */
        else
            continue;	/* continue searching */
    }
    
    if(i==NUMBER_OF_COMMANDS)
        return(INVALID_COMMAND); /* command not found in the database */
    
    return(SUCCESS);
}

