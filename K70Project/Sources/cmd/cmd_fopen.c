/*
 * cmd_fopen.c

 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include <stdio.h>
#include "../device/uart.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int cmd_fopen(int argc, char *argv[])
{
	int fd;
	char buffer[40];
	int len = 0;

	/* Accept 2 or 3 arguments */
	if(argc==1 || argc>=4)
		return(INVALID_INPUT);

	/* argv[0] = "fopen"
	 * argv[1] =  "file/device name" 
	 * argv[2] =  "w+" 
	 */

	switch(argc)
	{
	case 2:
		fd = svc_fopen_main(argv[1],"a");
		/* If nothing specified as argv[2] then file will be opened for reading and writing both, and will be 
		creating if does not exists. */
		break;

	case 3:
		fd = svc_fopen_main(argv[1],argv[2]);
		break;
	}

	if(fd>0)
	{
		len = snprintf(buffer,50,"Returned File Descriptor is: %d \r\n",fd);
		if(len>=0) svc_fputs_main(STDOUT,buffer);
	}
	else
	{	
		return(fd);
	}

	return(SUCCESS);
}
