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
#include "reportError.h"

int stringMatch(char**);

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
        case -1: /* case -1 represents to print all */
            /* Print All Help */
            
        case 0:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"DATE command will output to stdout the current date and time in the format 'January 23, 2014 15:57:07.123456'. It calls the POSIX system call 'gettimeofday' to determine the time and date.\n\n");
            fprintf(stdout,"No. of additional args : 0 \n");
            fprintf(stdout,"Example : > date \n");
            fprintf(stdout,"Output  : January 23, 2014 15:57:07.123456 \n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 1:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"ECHO command will output each of the arguments in argv (except for the first) to stdout with a single space character between the srguments. After the last argument is output, newline output is displayed.\n\n");
            fprintf(stdout,"No. of additional args : Any \n");
            fprintf(stdout,"Example : > echo This is a string \n");
            fprintf(stdout,"Output  : This is a string \n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 2:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"EXIT command will exit from the shell (i.e., cause the shell to terminate) by calling the exit system call.\n\n");
            fprintf(stdout,"No. of additional args : 0 \n");
            fprintf(stdout,"Example : > exit \n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 3:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"HELP command will output to stdout a brief description of the commands accepted by the shell.\n\n");
            fprintf(stdout,"No. of additional args : 1 or 2 \n");
            fprintf(stdout,"Example : > help \n");
            fprintf(stdout,"> help echo \n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 4:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"MALLOC command will can be used to allocate the memory of the number of byts specified.\n\n");
            fprintf(stdout,"No. of additional args : 1 \n");
            fprintf(stdout,"The argument can be specified either as a decimal integer constant (of length) or as a hexadecimal number (indicated by a prefix of 0x followed by an arbitrary length hexadecimal constant).\n");
            fprintf(stdout,"Example : > malloc 10 \n");
            fprintf(stdout,"Output : Storage allocated at: 0x700080 \n");
            fprintf(stdout,"Example : > malloc 0xFF \n");
            fprintf(stdout,"Output : Storage allocated at: 0x79A560 \n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 5:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"FREE command will can be used to de-allocate the memory which was allocated by malloc.\n\n");
            fprintf(stdout,"No. of additional args : 1 \n");
            fprintf(stdout,"The argument can be specified either as a decimal integer constant (of arbitrary length) or as a hexadecimal number (indicated by a prefix of 0x followed by an arbitrary length hexadecimal constant).\n");
            fprintf(stdout,"Example : > free 0x700080 \n");
            fprintf(stdout,"Output : The command executed with return status 0. \n");
            fprintf(stdout,"Example : > free 255 \n");
            fprintf(stdout,"Output : The command executed with return status 0. \n");
            fprintf(stdout,"NOTE : To see if the memory is actually freed use memorymap command to visualize the map.\n\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            
        case 6:
            fprintf(stdout,"***********************************************************************\n");
            fprintf(stdout,"MEMORYMAP command will can be used visualize the memory which was allocated by malloc.\n\n");
            fprintf(stdout,"No. of additional args : 0 \n");
            fprintf(stdout,"Example : > memorymap \n");
            fprintf(stdout,"Output : \n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout,"       Main Memory Starts\n");
            fprintf(stdout,"           Head: 0x7fb21c077010\n");
            fprintf(stdout,"  Metadata size: 16 Bytes\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout," PID          = 0\n");
            fprintf(stdout," isFree       = 0\n");
            fprintf(stdout," Size         = 16 Bytes\n");
            fprintf(stdout," Data Address = 0x7fb21c077020\n");
            fprintf(stdout," Next Block   = 0x7fb21c077210\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout,"PID          = 0\n");
            fprintf(stdout,"isFree       = 1\n");
            fprintf(stdout,"Size         = 48 Bytes\n");
            fprintf(stdout,"Data Address = 0x7fb21c077220\n");
            fprintf(stdout,"Next Block   = 0x7fb21c077610\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout," PID          = 0\n");
            fprintf(stdout," isFree       = 0\n");
            fprintf(stdout," Size         = 32 Bytes\n");
            fprintf(stdout," Data Address = 0x7fb21c077620\n");
            fprintf(stdout," Next Block   = 0x7fb21c077910\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout," PID          = 0\n");
            fprintf(stdout," isFree       = 1\n");
            fprintf(stdout," Size         = 1999840 Bytes\n");
            fprintf(stdout," Data Address = 0x7fb21c077920\n");
            fprintf(stdout," Next Block   = (nil)\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout,"     Main Memory Ends\n");
            fprintf(stdout,"-----------------------------------\n");
            fprintf(stdout,"***********************************************************************\n");
            if(!printAll)break;
            break; /* Break at the last case */
            
        default:
            return(INVALID_COMMAND);
    }
    return(SUCCESS);
}
