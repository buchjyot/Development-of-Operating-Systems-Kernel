/*
 * lcdcRoutines.c
 *
 *  Created on: Apr 9, 2017
 *      Author: welcome
 */

#include "../util/reportError.h"
#include "deviceFcnDeclaration.h"
#include "lcdcConsole.h"

struct console console;

int lcdc_fgetc(void *ptr)
{
	return(OUPUT_ONLY_DEVICE);
}

int lcdc_fputc(char c, void *ptr)
{
	// Output the character on the TWR_LCD_RGB
	lcdcConsolePutc(&console, c);

	return(SUCCESS);
}

int lcdc_fopen(void *ptr)
{	
	lcdcConsoleInit(&console);
	return(SUCCESS);
}

int lcdc_fclose(void *ptr)
{
	lcdcConsoleClear(&console);
	return(SUCCESS);
}

void consoleInit()
{
	/* lcdcConsoleInit() */
	lcdcConsoleInit(&console);
}

