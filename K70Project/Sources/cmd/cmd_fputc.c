/*
 * cmd_fputc.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/util.h"
#include "../util/reportError.h"
#include <stdlib.h>
#include "../svc/svc.h"

int cmd_fputc(int argc, char *argv[])
{
	int fd,len;
	errorCode retCode;
	char *eptr;
	char c = argv[2][0];

	/* Strictly accept 3 arguments */
	if(!(argc==3))
		return(INVALID_INPUT);

	/* argv[0] = "fputc"
	 * argv[1] = 0   //file Descriptor 
	 * argv[2] = 50
	 */
	
	len = mystrlen(argv[2]);
	if(len!=1)svc_reportError(ONLY_BYTE_LEVEL_OPERATIONS);
		
	/* Convert char to integer */
	fd = strtol(argv[1],&eptr,10);

	retCode = svc_fputc_main(fd,c);	
	return(retCode);
}
