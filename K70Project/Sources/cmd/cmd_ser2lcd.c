/*
 * cmd_ser2lcd.c
 *
 *  Created on: Apr 9, 2017
 *      Author: Jyot Buch
 *      
 *      ser2lcd: Continuously copy characters from serial input to
        LCD.  End on a ^D (control-D) input character.
 */
#include "../util/reportError.h"
#include "../svc/svc.h"

#define CHAR_EOF 4

int cmd_ser2lcd(int argc, char *argv[])
{
	int returnCode = SUCCESS;
	int c;
	
	/* Strictly accept no other arguments */
	if(!(argc==1))
		return(INVALID_INPUT);

	int lcdfD = svc_fopen_main("/dev/lcdc/rgb","w");
	
	while(1)
	{
		c = svc_fgetc_main(STDIN);
		
		/* End on a ^D (control-D) input character*/
		if(c == CHAR_EOF)
		{
			svc_fputs_main(STDOUT,"\r\n");
			returnCode = SUCCESS;
			break;
		}
		
		/* Continuously copy characters from serial input to LCD.*/
		returnCode = svc_fputc_main(STDOUT,(char)c);
		if(returnCode!=SUCCESS)break;
		
		returnCode = svc_fputc_main(lcdfD,(char)c);
		if(returnCode!=SUCCESS)break;
	}
	
	svc_fclose_main(lcdfD);
	return(returnCode);
}
