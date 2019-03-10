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
#include "../util/reportError.h"
#include "../device/uart.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int cmd_echo(int argc, char *argv[])
{
	int i;
	int len = 0;
	char buffer[256];

	/* If there are no arguments with echo then do not print anything just
	 * return success.*/
	if(argc==1)
		return(SUCCESS);

	/* Starting loop from 1, since we need to ignore 1st argument which
	 * will be echo */
	for(i=1;i<argc-1;i++)
	{
		len = snprintf(buffer,256,"%s ",argv[i]);
		if(len>=0) svc_fputs_main(STDOUT,buffer);
	}

	/* print the new line character after last argument */

	len = snprintf(buffer,256,"%s",argv[i]);
	if(len>=0) svc_fputs_main(STDOUT,buffer);

	svc_fputs_main(STDOUT,"\r\n");	
	return(SUCCESS);
}
