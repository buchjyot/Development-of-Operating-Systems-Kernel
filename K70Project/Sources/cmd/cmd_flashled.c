/*
 * cmd_flashled.c
 *
 *  Created on: Apr 15, 2017
 *      Author: welcome
 */

/* "flashled" that uses the user
 * timer supervisor calls in problem "c" above to flash the orange
 * LED on and off every half a second (the LED will light once a
 * second). */

#include "../util/reportError.h"
#include "../svc/svc.h"
#include "../int/PDB0.h"
#include "../util/delay.h"

void myUserFunction(void);

int cmd_flashled(int argc, char *argv[])
{
	/* Strictly accept 1 argument */
	if(!(argc==1))
		return(INVALID_INPUT);

	int pb1fd = SVCfopen_mainImpl("/dev/pushbutton/pb1","r");

	/* provide user instruction on how to operate with hardware */
	svc_fputs_main(STDOUT,"Flash LED program is Running... \r\nPlease press pushbuttons 1 to stop execution. \r\n");

	/* set the time for 0.5 sec = 500 milli seconds and call this function as an interrupt routine */
	svc_setPDBTimer(500,&myUserFunction,PDBTimerContinuous);

	/* Monitor the status of pushbutton and if SW1 is depressed then turn off the timer*/
	while(1)
	{
		if(svc_fgetc_main(pb1fd))
		{
			/* Stop the timer by sending the 0 time */
			svc_setPDBTimer(0,NULL,0);

			break;
		}
	}

	SVCfclose_mainImpl(pb1fd);
	return(SUCCESS);
}

void myUserFunction(void)
{
	/* Notice this this user function will be called from a supervisor call so we are having all the Impl versions of functions */ 
	static char ledState = 0;

	int orangefd = SVCfopen_mainImpl("/dev/led/orange","w");

	if((ledState ^= 1) == 1)
		SVCfputc_mainImpl(orangefd,1);
	else
		SVCfputc_mainImpl(orangefd,0);

	SVCfclose_mainImpl(orangefd);
}
