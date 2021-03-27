/*
 * uartRoutines.c
 *
 *  Created on: Mar 23, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include "../int/intSerialIO.h"
#include "uart.h"

/********************************************************************/
/*
 * Wait for space in the specified UART Tx FIFO and then output a character
 *
 * Parameters:
 *  c            character to be output
 */
int uart_fputc(char c, void* ptr)
{
	putcharIntoBuffer(c);
	return(SUCCESS);
}

/********************************************************************/
/*
 * Wait for and read a received character from the specified UART
 *
 * Parameters:
 *  uartChannel  UART channel on which to perform input
 *
 * Return Values:
 *  the received character
 */
int uart_fgetc(void* ptr)
{

	/* Return the 8-bit data from the receiver */
	return (unsigned char) getcharFromBuffer();
}

int uart_fopen(void* ptr)
{
	/* Setup UART */
	errorCode rootStatus = uartSetup();
	return(rootStatus);
}

int uart_fclose(void* myLED)
{
	/* This function does nothing, since in our case we can not turn off the UART, otherwise all the shell interaction will go away.*/
	return(SUCCESS);
}
