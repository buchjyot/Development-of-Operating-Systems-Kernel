/*
 * fputs.c
 *
 *  Created on: Apr 8, 2017
 *      Author: Jyot Buch
 */

#include "../util/util.h"
#include "../util/reportError.h"
#include <stdlib.h>
#include "../svc/svc.h"

int cmd_fputs(int argc, char *argv[])
{
	int fd,i,j,k=0;
	errorCode retCode = SUCCESS;
	char *eptr;
	char tempString[256];

	/* Accept 3 or more arguments */
	if(argc<3)
		return(INVALID_INPUT);

	/* argv[0] = "fputs"
	 * argv[1] = 0   //file Descriptor 
	 * argv[2] = This
	 * argv[3] = is
	 * argv[4] = OS.
	 */

	/* Convert char to integer */
	fd = strtol(argv[1],&eptr,10);

	/* Construct the tempString */
	for(i = 2; i < argc; i++)
	{
		for(j = 0; j < mystrlen(argv[i]); j++)
		{
			tempString[k] = argv[i][j];		
			k++;
		}

		/* Since argv are separated by space */
		tempString[k] = ' ';
		k++;
	}

	/* terminate tempString */
	tempString[k] = '\0';
	
	/* Pass the string to fputs_main fcn */
	retCode = svc_fputs_main(fd,tempString);

	/* line break at the end of the content if fd ==2 or fd ==3 for better formatting in UART.*/
	return(retCode);
}
