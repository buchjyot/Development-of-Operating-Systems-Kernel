/*
 * cmd_exit command : When user inputs "exit" command this command is being
 *					 executed.
 *
 * Expected output:
 *
 *	> exit
 *
 * This program is mainly used to terminate the shell execution.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <stdlib.h>
#include "reportError.h"

int cmd_exit(int argc, char *argv[])
{
    /* Using EXIT system call */
    if(argc==1)
    {
        exit(0);
        return(SUCCESS);
    }
    else
        return(INVALID_INPUT);
}
