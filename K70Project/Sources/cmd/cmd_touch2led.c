/**
 * main.c

 * Capacitive touch pads demonstration project main program
 *
 * Demonstrates using the K70 capacitive touch pads
 * 
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-251 Fall 2012, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

#define ASCII_FALSE 48

#include <stdio.h>
#include "derivative.h"
#include "../device/led.h"
#include "../device/pushbutton.h"
#include "../util/reportError.h"
#include "../svc/svc.h"
#include "../device/deviceEnums.h"

/* touch2led: Continuously copy from each touch sensor to the
 * corresponding LED.  End when all four touch sensors are
 * "depressed."*/

int cmd_touch2led(int argc, char *argv[])
{
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	/* provide user instruction on how to operate with hardware */
	svc_fputs_main(STDOUT,"Capacitive TouchPad-LED Project Running... \r\nPlease press all the touch sensors to stop execution.\r\n");

	int orangefid = svc_fopen_main("/dev/led/orange","w");
	int yellowfid = svc_fopen_main("/dev/led/yellow","w");
	int bluefid = svc_fopen_main("/dev/led/blue","w");
	int greenfid = svc_fopen_main("/dev/led/green","w");

	int electrode1ID = svc_fopen_main("/dev/touch/ts1","r");
	int electrode2ID = svc_fopen_main("/dev/touch/ts2","r");
	int electrode3ID = svc_fopen_main("/dev/touch/ts3","r");
	int electrode4ID = svc_fopen_main("/dev/touch/ts4","r");

	while(1)
	{
		if(svc_fgetc_main(electrode1ID))
		{svc_fputc_main(orangefid,ON);}
		else 
		{svc_fputc_main(orangefid,OFF);}

		if(svc_fgetc_main(electrode2ID))
		{svc_fputc_main(yellowfid,ON);}
		else
		{svc_fputc_main(yellowfid,OFF);}

		if(svc_fgetc_main(electrode3ID)) 
		{svc_fputc_main(greenfid,ON);} 
		else 
		{svc_fputc_main(greenfid,OFF);}

		if(svc_fgetc_main(electrode4ID))
		{svc_fputc_main(bluefid,ON);} 
		else 
		{svc_fputc_main(bluefid,OFF);}

		/* Break the loop when all the touch sensors are pressed */
		if(svc_fgetc_main(electrode1ID) && svc_fgetc_main(electrode2ID) && svc_fgetc_main(electrode3ID) && svc_fgetc_main(electrode4ID))
		{
			svc_fputc_main(greenfid,OFF);
			svc_fputc_main(yellowfid,OFF);
			svc_fputc_main(orangefid,OFF);
			svc_fputc_main(bluefid,OFF);
			break;
		}
	}

	svc_fclose_main(orangefid);
	svc_fclose_main(greenfid);
	svc_fclose_main(yellowfid);
	svc_fclose_main(bluefid);

	svc_fclose_main(electrode1ID);
	svc_fclose_main(electrode2ID);
	svc_fclose_main(electrode3ID);
	svc_fclose_main(electrode4ID);

	return(SUCCESS);
}
