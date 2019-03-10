/*
 * cmd_echo command : When user inputs "echo" command this command is being
 *					 executed.
 *
 * Expected output:
 *
 *	> echo this is a string
 *	this is a string
 *
 * As any other program this program accepts the argc and argv as an arguments. It
 * checks the validity of the input and displays the argv ouput in the shell.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <stdio.h>
#include "reportError.h"

int cmd_echo(int argc, char *argv[])
{
    int i;
    
    /* If there are no arguments with echo then do not print anything just
     * return success.*/
    if(argc==1)
        return(SUCCESS);
    
    /* Starting loop from 1, since we need to ignore 1st argument which
     * will be echo */
    for(i=1;i<argc-1;i++)
    {
        fprintf(stdout,"%s ",argv[i]);
    }
    
    /* print the new line character after last argument */
    fprintf(stdout,"%s\n",argv[i]);
    return(SUCCESS);
}
