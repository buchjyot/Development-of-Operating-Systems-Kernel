/*
 * cmd_pb2led.c
 *
 *  Created on: Apr 9, 2017
 *      Author: Jyot Buch
 *      
 *      pb2led: Continuously copy from SW1 to orange LED and SW2 to
 *      yellow LED.  End when both SW1 and SW2 are depressed.
 */

#include "../util/reportError.h"
#include "../svc/svc.h"
#include "../util/delay.h"

int cmd_pb2led(int argc, char *argv[])
{
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	/* provide user instruction on how to operate with hardware */
	svc_fputs_main(STDOUT,"PushButton-LED Project Running... \r\nPlease press both the pushbuttons to stop execution.\r\n");

	int returnCode = SUCCESS;
	int pb1fd = svc_fopen_main("/dev/pushbutton/pb1","r");
	int pb2fd = svc_fopen_main("/dev/pushbutton/pb2","r");
	int orangefd = svc_fopen_main("/dev/led/orange","w");
	int yellowfd = svc_fopen_main("/dev/led/yellow","w");

	while(1)
	{
		if(svc_fgetc_main(pb1fd)&&svc_fgetc_main(pb2fd))
		{	
			/* In case one of the led if lit then turn off the led*/
			svc_fputc_main(orangefd,OFF);
			svc_fputc_main(yellowfd,OFF);
			break;
		}

		if(svc_fgetc_main(pb1fd))
		{svc_fputc_main(orangefd,ON);}
		else 
		{svc_fputc_main(orangefd,OFF);}

		if(svc_fgetc_main(pb2fd))
		{svc_fputc_main(yellowfd,ON);}
		else
		{svc_fputc_main(yellowfd,OFF);}
	}

	/* Close devices */
	svc_fclose_main(pb1fd);
	svc_fclose_main(pb2fd);	
	svc_fclose_main(orangefd);
	svc_fclose_main(yellowfd);

	return(returnCode);
}
