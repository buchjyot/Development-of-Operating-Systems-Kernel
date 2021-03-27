/*
 * cmd_whoami.c
 *
 *  Created on: Apr 30, 2017
 *      Author: Jyot Buch
 */
#include <stdlib.h>
#include "../util/util.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int cmd_whoami(int argc, char *argv[])
{
	int len = 0;
	char buffer[256];

	if(argc==1)
	{
		len = snprintf(buffer,256,"%s\r\n",currentUser);
		if(len>=0) svc_fputs_main(STDOUT,buffer);
		return(SUCCESS);
	}
	else
		return(INVALID_INPUT);
}

