/*
 * cmd_fclose.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include <stdlib.h>
#include "../util/util.h"
#include "../svc/svc.h"

int cmd_fclose(int argc, char *argv[])
{
	int fd,len;
	errorCode ec;
	char* eptr;

	/* Strictly accept 2 arguments */
	if(!(argc==2))
		return(INVALID_INPUT);

	/* Second value is a stream ID */
	/* argv[0] = "fclose"
	 * argv[1] =  "4" 
	 */

	len = mystrlen(argv[1]);
	if(len!=1)return(INVALID_FILE_DISCRIPTOR);

	fd = strtoul(argv[1],&eptr,10);

	ec = svc_fclose_main(fd);
	return(ec);
}

