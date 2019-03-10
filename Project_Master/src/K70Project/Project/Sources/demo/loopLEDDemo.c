/**
 * main.c
 * FlashLED project main program
 *
 * Demonstrates using LEDs and delaying via looping
 * 
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 *
 * Copyright © 2017, 2015, 2014, 2012 James L. Frankel.  All rights reserved.
 */
#include "../util/delay.h"
#include "../svc/svc.h"
#include "../device/deviceEnums.h"
#include "../util/reportError.h"

int loopLEDDemo(int argc, char *argv[])
{
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	const unsigned long int delayCount = 1000000;

	/* provide user instruction on how to operate with hardware */
	svc_fputs_main(STDOUT,"LED Loop demo is Running... \r\nPlease press pushbuttons 1 to stop execution. \r\n");

	int orangefid = svc_fopen_main("/dev/led/orange","w");
	int yellowfid = svc_fopen_main("/dev/led/yellow","w");
	int bluefid = svc_fopen_main("/dev/led/blue","w");
	int greenfid = svc_fopen_main("/dev/led/green","w");
	int pb1fd = svc_fopen_main("/dev/pushbutton/pb1","r");

	while(1)
	{
		if(svc_fgetc_main(pb1fd))
		{	
			/* In case one of the led if lit then turn off the led*/
			svc_fputc_main(orangefid,OFF);
			svc_fputc_main(yellowfid,OFF);
			svc_fputc_main(bluefid,OFF);
			svc_fputc_main(greenfid,OFF);
			break;
		}

		delay(delayCount);	
		svc_fputc_main(orangefid,ON);
		delay(delayCount);	
		svc_fputc_main(yellowfid,ON);
		delay(delayCount);	
		svc_fputc_main(greenfid,ON);
		delay(delayCount);	
		svc_fputc_main(bluefid,ON);

		delay(delayCount);	
		svc_fputc_main(bluefid,OFF);
		delay(delayCount);	
		svc_fputc_main(greenfid,OFF);
		delay(delayCount);	
		svc_fputc_main(yellowfid,OFF);
		delay(delayCount);	
		svc_fputc_main(orangefid,OFF);
	}

	svc_fclose_main(orangefid);
	svc_fclose_main(bluefid);
	svc_fclose_main(greenfid);
	svc_fclose_main(yellowfid);
	svc_fclose_main(pb1fd);

	return(SUCCESS);
}
