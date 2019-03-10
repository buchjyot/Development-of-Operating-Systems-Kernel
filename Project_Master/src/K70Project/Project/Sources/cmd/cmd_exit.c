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
#include "../util/reportError.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int cmd_exit(int argc, char *argv[])
{
    /* Using EXIT system call */
    if(argc==1)
    {
    	svc_fputs_main(STDOUT,"Process Exited Successfully.\r\n");
        exit(0);
        return(SUCCESS);
    }
    else
        return(INVALID_INPUT);
}
