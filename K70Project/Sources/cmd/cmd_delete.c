/*
 * cmd_delete.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include "../svc/svc.h"

int delete_file(char*);

int cmd_delete(int argc, char *argv[])
{
	errorCode ret;
    
    /* Strictly accept 2 arguments */
    if(!(argc==2))
        return(INVALID_INPUT);
	
	ret = svc_delete(argv[1]);
    return(ret);
}

