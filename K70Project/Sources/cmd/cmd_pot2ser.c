/*
 * cmd_pot2ser.c
 *
 *  Created on: Apr 9, 2017
 *      Author: Jyot Buch
 *      
 *      pot2ser: Continuously output the value of the analog
 *      Potentiometer to the serial device as a decimal or
 *      hexadecimal number followed by a newline.  End when SW1 is
 *      depressed.
 */

#include "../util/reportError.h"
#include "../svc/svc.h"
#include "../util/delay.h"

int cmd_pot2ser(int argc, char *argv[])
{
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	int returnCode = SUCCESS;
	unsigned int reading;
	char buffer[50];
	int len;
	int pbfd = svc_fopen_main("/dev/pushbutton/pb1","r");
	int potfd = svc_fopen_main("/dev/adc/pot","r");

	while(1)
	{
		/* get the reading from pot */
		reading = svc_fgetc_main(potfd);

		if(svc_fgetc_main(pbfd))
		{
			returnCode = SUCCESS;
			break;
		}

		/* Display formated string to UART2 */
		len = snprintf(buffer,50,"Potentiometer Reading: %4.2u \r\n",reading);
		if(len>=0) returnCode = svc_fputs_main(STDOUT,buffer);		
		if(returnCode!=SUCCESS)break;

		/* Take reading at certain interval */
		delay(400000);
	}

	/* Close devices */
	svc_fclose_main(pbfd);
	svc_fclose_main(potfd);	

	return(returnCode);
}
