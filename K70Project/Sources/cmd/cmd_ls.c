/*
 * cmd_ls.c
 *
 *  Created on: Apr 9, 2017
 *      Author: welcome
 */
#include "../util/reportError.h"
int listFiles(void);

int cmd_ls(int argc, char *argv[])
{
    int status;
    
    /* Strictly accept only 1 arguments */
    if(!(argc==1))
        return(INVALID_INPUT);
    
    /* argv[0] = "ls"
     */
    status = listFiles();
    
    return(status);
}
