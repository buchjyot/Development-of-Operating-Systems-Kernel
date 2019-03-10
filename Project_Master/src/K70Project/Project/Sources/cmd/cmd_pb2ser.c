/*
 * cmd_pb2ser.c
 *
 *  Created on: May 7, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include "../svc/svc.h"

int cmd_pb2ser(int argc, char *argv[])
{
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	/* provide user instruction on how to operate with hardware */
	svc_fputs_main(STDOUT,"PushButton-to-Serial Project Running... \r\n");

	int returnCode = SUCCESS;
	int pb2fd = svc_fopen_main("/dev/pushbutton/pb2","r");
	int pb1fd = svc_fopen_main("/dev/pushbutton/pb1","r");
	
	while(1)
	{
		if(svc_fgetc_main(pb2fd))
		{	
			while(svc_fgetc_main(pb2fd)){};

			/* Transmit the message that pushbutton2 was depressed */
			returnCode = svc_fputs_main(STDOUT,"Pushbutton2 is depressed. \r\n");
			if(returnCode!=SUCCESS)break;
		}

		if(svc_fgetc_main(pb1fd))
			break;
	}

	/* Close devices */
	svc_fclose_main(pb2fd);	
	svc_fclose_main(pb1fd);	

	return(returnCode);
}
