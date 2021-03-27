/*
 * cmd_help command : When user inputs "help" command this command is being
 *					 executed.
 *
 * Expected output:
 *
 *	> help echo
 *		ECHO command will output each of the arguments in argv (except for the first) to stdout with a
 *    	single space character between the srguments. After the last argument is output, a newline output
 * 		is displayed.
 *		Number of arguments that are accepted : Any
 *		Example : > echo This is a string
 *
 *   Following command will print all possible help for all the command.
 *  	> help
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <stdio.h>
#include "../util/reportError.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"
#include "../util/util.h"
#include "helpEnums.h"

void displayAllCommands(void);

int cmd_help(int argc, char *argv[])
{
	int id;
	int printAll = 0;

	if(argc > 2)
		return(INVALID_INPUT);

	switch(argc)
	{
	case 1:
		printAll = 1;
		id = -1;
		break;

	case 2:
		id = stringMatch(argv+1);
		break;
	}

	switch(id)
	{
	case PRINT_ALL: /* case -1 represents to print all */
		/* Print All Help */
		svc_fputs_main(STDOUT,"============================\r\n");
		svc_fputs_main(STDOUT,"List of available commands:\r\n");	
		svc_fputs_main(STDOUT,"============================\r\n");
		displayAllCommands();		
		svc_fputs_main(STDOUT,"============================\r\n");
		if(printAll)break;

	case DATE:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"DATE command will output to stdout the current date and time in the format 'January 23, 2014 15:57:07'.We maintain the epoch count and convert that value to date format.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > date \r\n");
		svc_fputs_main(STDOUT,"Output  : January 23, 2014 15:57:07.123456 \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > date 1234567778000 \r\n");
		svc_fputs_main(STDOUT,"Output : The command executed with return status 0. \r\n");
		svc_fputs_main(STDOUT,"NOTE: Only privileged administrator logins are allowed to set the date and time clock for the system.\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case ECHO:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"ECHO command will output each of the arguments in argv (except for the first) to stdout with a single space character between the srguments. After the last argument is output, newline output is displayed.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : Any \r\n");
		svc_fputs_main(STDOUT,"Example : > echo This is a string \r\n");
		svc_fputs_main(STDOUT,"Output  : This is a string \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case EXIT:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"EXIT command will exit from the shell (i.e., cause the shell to terminate) by calling the exit system call.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 0 \r\n");
		svc_fputs_main(STDOUT,"Example : > exit \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case HELP:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"HELP command will output to stdout a brief description of the commands accepted by the shell.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > help \r\n");
		svc_fputs_main(STDOUT,"> help echo \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case MALLOC:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"MALLOC command will can be use to allocate the memory of the number of byts specified.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"The argument can be specified either as a decimal integer constant (of length) or as a hexadecimal number (indicated by a prefix of 0x followed by an arbitrary length hexadecimal constant).\r\n");
		svc_fputs_main(STDOUT,"Example : > malloc 10 \r\n");
		svc_fputs_main(STDOUT,"Output : Storage allocated at: 0x700080 \r\n");
		svc_fputs_main(STDOUT,"Example : > malloc 0xFF \r\n");
		svc_fputs_main(STDOUT,"Output : Storage allocated at: 0x79A560 \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FREE:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FREE command will can be use to de-allocate the memory which was allocated by malloc.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"The argument can be specified either as a decimal integer constant (of arbitrary length) or as a hexadecimal number (indicated by a prefix of 0x followed by an arbitrary length hexadecimal constant).\r\n");
		svc_fputs_main(STDOUT,"Example : > free 0x700080 \r\n");
		svc_fputs_main(STDOUT,"Output : The command executed with return status 0. \r\n");
		svc_fputs_main(STDOUT,"Example : > free 255 \r\n");
		svc_fputs_main(STDOUT,"Output : The command executed with return status 0. \r\n");
		svc_fputs_main(STDOUT,"NOTE : To see if the memory is actually freed use memorymap command to visualize the map.\r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FREEERRORCODE:
		/* freeerrorcode */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FREEERRORCODE command will can be use to de-allocate the memory which was allocated by malloc.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"The argument can be specified either as a decimal integer constant (of arbitrary length) or as a hexadecimal number (indicated by a prefix of 0x followed by an arbitrary length hexadecimal constant).\r\n");
		svc_fputs_main(STDOUT,"The only difference is that free do not return any error code, but freeerrorcode command does return some error code so that user knows what happened in the free command.\r\n");
		svc_fputs_main(STDOUT,"Example : > freeerrorcode 0x700080 \r\n");
		svc_fputs_main(STDOUT,"Output : The command executed with return status 0. \r\n");
		svc_fputs_main(STDOUT,"Example : > freeerrorcode 255 \r\n");
		svc_fputs_main(STDOUT,"Output : You are attempting to free the storage that you have not allocated.\r\n");
		svc_fputs_main(STDOUT,"NOTE : To see if the memory is actually freed use memorymap command to visualize the map.\r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case MEMORYMAP:
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"MEMORYMAP command will can be use visualize the memory which was allocated by malloc.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 0 \r\n");
		svc_fputs_main(STDOUT,"Example : > memorymap \r\n");
		svc_fputs_main(STDOUT,"Output : \r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT,"       Main Memory Starts\r\n");
		svc_fputs_main(STDOUT,"           Head: 0x7fb21c077010\r\n");
		svc_fputs_main(STDOUT,"  Metadata size: 16 Bytes\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT," PID          = 0\r\n");
		svc_fputs_main(STDOUT," isFree       = 0\r\n");
		svc_fputs_main(STDOUT," Size         = 16 Bytes\r\n");
		svc_fputs_main(STDOUT," Data Address = 0x7fb21c077020\r\n");
		svc_fputs_main(STDOUT," Next Block   = 0x7fb21c077210\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT,"PID          = 0\r\n");
		svc_fputs_main(STDOUT,"isFree       = 1\r\n");
		svc_fputs_main(STDOUT,"Size         = 48 Bytes\r\n");
		svc_fputs_main(STDOUT,"Data Address = 0x7fb21c077220\r\n");
		svc_fputs_main(STDOUT,"Next Block   = 0x7fb21c077610\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT," PID          = 0\r\n");
		svc_fputs_main(STDOUT," isFree       = 0\r\n");
		svc_fputs_main(STDOUT," Size         = 32 Bytes\r\n");
		svc_fputs_main(STDOUT," Data Address = 0x7fb21c077620\r\n");
		svc_fputs_main(STDOUT," Next Block   = 0x7fb21c077910\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT," PID          = 0\r\n");
		svc_fputs_main(STDOUT," isFree       = 1\r\n");
		svc_fputs_main(STDOUT," Size         = 1999840 Bytes\r\n");
		svc_fputs_main(STDOUT," Data Address = 0x7fb21c077920\r\n");
		svc_fputs_main(STDOUT," Next Block   = (nil)\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT,"     Main Memory Ends\r\n");
		svc_fputs_main(STDOUT,"-----------------------------------\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case MEMTEST:
		/* memtest*/ 
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"MEMTEST is to test the memory allocation and deallocation (ONLY TEST PURPOSE).\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 0 \r\n");	
		svc_fputs_main(STDOUT,"Example : > memtest 0x700080 \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FGETC:
		/* fgetc */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FGETC command will output the character from the file/device. This function returns the character read as an unsigned char cast to an int.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > fgetc 0 \r\n");
		svc_fputs_main(STDOUT,"Output  :Returned character is: 50 \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FPUTC: 
		/* fputc */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FPUTC command will put the character to the file/device.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"NOTE: Following command will do stdout to UART.\r\n");
		svc_fputs_main(STDOUT,"Example : > fputc 1 50 \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FOPEN:
		/* fopen */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FOPEN command will open the file/device and will return its file descriptor.\r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > fopen /dev/led/blue \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > fopen stdin r \r\n");
		svc_fputs_main(STDOUT,"Output  :You are trying to open stdin/stdout/stderr streams which are already open. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > fopen myTestFile e \r\n");
		svc_fputs_main(STDOUT,"Output  :Invalid mode: Please provide valid mode from {r,r+,w,w+,a}. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case FCLOSE:
		/* fclose */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FCLOSE command will close the file/device.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > fclose 4 \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case CREATE:
		/* create */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"CREATE command will create a file.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > create myTestFile \r\n");
		svc_fputs_main(STDOUT,"Output  :Returned File Descriptor is: 4. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;

	case DELETE: 
		/* delete */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"DELETE command will delete a file.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > delete myTestFile \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case LS: 
		/* ls */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"LS command will list available files.\r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > ls \r\n");
		svc_fputs_main(STDOUT,"            mathworks \r\n");
		svc_fputs_main(STDOUT,"            student_data \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break; 

	case FPUTS: 
		/* fputs */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FPUTS command will put a string in to a file.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : >=2 \r\n");
		svc_fputs_main(STDOUT,"Example : > fputs 3 This is a string \r\n");
		svc_fputs_main(STDOUT,"Output  :The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break; 

	case FGETS: 
		/* delete */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"FGETS command will read a string from file until we reach end of the file.\r\n\r\n");
		svc_fputs_main(STDOUT,"No. of additional args : 1 \r\n");
		svc_fputs_main(STDOUT,"Example : > fgets 3 \r\n");
		svc_fputs_main(STDOUT,"Output  :This is a string The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break; 

	case LEDLOOP:
		/* ledflash */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"LEDLOOP command will flash all the leds. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > ledloop \r\n");
		svc_fputs_main(STDOUT,"NOTE  :This will be modified to flashled in next problemset \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break; 

	case TOUCH2LED:
		/* touch2led */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"TOUCH2LED  Continuously copy from each touch sensor to the corresponding LED.  End when all four touch sensors are depressed. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > touch2led \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break; 

	case POT2SER:
		/* pot2ser */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"POT2SER Continuously output the value of the analog potentiomemter to the serial device as a decimal or hexadecimal number followed by a newline.  End when SW1 is depressed. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > pot2ser \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case THERM2SER:
		/* therm2ser */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"THERM2SER Continuously output the value of the thermistor to the serial device as a decimal or hexadecimal number followed by a newline.  End when SW1 is depressed. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > therm2ser \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case PB2LED:
		/* pb2led */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"PB2LED Continuously copy from SW1 to orange LED and SW2 to yellow LED.  End when both SW1 and SW2 are depressed. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > pb2led \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case SER2LCD:
		/* ser2lcd */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"SER2LCD Continuously copy characters from serial input to LCD.  End on a ^D (control-D) input character. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > ser2lcd \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case EXAMINE:
		/* examine */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"EXAMINE displays the content of memory location in increment of 2 bytes. It also display its ASCII values. Any non-printing ASCII characters (i.e. those with values from 0 to 31 decimal (0x1f), inclusive, and from 127 decimal (0x7f) to 255 decimal (0xff), inclusive) will be displayed as a period.\r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > examine 0x80000090 \r\n");
		svc_fputs_main(STDOUT,"Addr      +0   +2   +4   +6   +8   +a   +c   +e    0 2 4 6 8 a c e \r\n");
		svc_fputs_main(STDOUT,"80000090  436f 6e74 656e 7473 206f 6620 0002 7f94  Contents of .... \r\n");
		svc_fputs_main(STDOUT,"NOTE: To test this command please use malloc block. \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case DEPOSIT:
		/* deposit */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"DEPOSIT allows user to write bytes to memory locaitons. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > deposit 0x80000080 78 101 119 32 109 101 109 11 114 121 33 \r\n");
		svc_fputs_main(STDOUT,"Output  : The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case FLASHLED:
		/* flashled */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"flashled uses user timer for 0.5 second period and blinks the orange LED. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > flashled \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case WHOAMI:
		/* whoami */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"WHOAMI returns the user name of the current login. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > whoami \r\n");
		svc_fputs_main(STDOUT,"Output  : guest \r\n");
		svc_fputs_main(STDOUT,"Output  : The command executed with return status 0. \r\n\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case MULTITASK:
		/* multitask */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"MULTITASK runs the multitasking enviornment test program. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > multitask \r\n");
		svc_fputs_main(STDOUT,"Runs 3 processes in multitasking mode. \r\n");
		svc_fputs_main(STDOUT,"NOTE: This command is not supported yet, it will return success always.\r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case PB2SER:
		/* pb2ser */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"PB2SER prints the status of pb2 if it is pressed. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > pb2ser \r\n");
		svc_fputs_main(STDOUT,"NOTE: To terminate this command press pb1. \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	case TASKLIST:
		/* tasklist */
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		svc_fputs_main(STDOUT,"TASKLIST prints the currently running process information. \r\n\r\n");
		svc_fputs_main(STDOUT,"Example : > tasklist \r\n");
		svc_fputs_main(STDOUT,"***********************************************************************\r\n");
		if(!printAll)break;
		break;

	default:
		return(INVALID_COMMAND);
	}
	return(SUCCESS);
}
