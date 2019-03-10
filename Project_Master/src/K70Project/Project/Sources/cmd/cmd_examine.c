/*
 * cmd_examine.c
 *
 *  Created on: Apr 15, 2017
 *      Author: Jyot Buch
 *      
 *      
 *   The command will display 16 bytes starting at the requested
     address.  The memory locations will be displayed in both
     hexadecimal and ASCII.  The hexadecimal output will be displayed
     in groups of two bytes.  The ASCII output will be displayed with
     no intervening spaces.  Any non-printing ASCII characters (i.e.,
     those with values from 0 to 31 decimal (0x1f), inclusive, and
     from 127 decimal (0x7f) to 255 decimal (0xff), inclusive) should
     be displayed as a period.  The examine command should accept a
     single required argument which is the address of the first
     location in memory to be displayed.  The argument should be able
     to be specified either as a decimal integer constant (of
     arbitrary length), an octal integer constant (indicated by a
     prefix of 0 not followed by x followed by an arbitrary length
     octal constant), or as a hexadecimal number (indicated by a
     prefix of 0x followed by an arbitrary length hexadecimal
     constant).  The alphabetic hexadecimal digits should be able to
     be specified in either upper or lower case.  This command should
     output two lines to stdout in the following format:

     $ examine 0x80000080
     Addr      +0   +2   +4   +6   +8   +a   +c   +e    0 2 4 6 8 a c e
     80000080  436f 6e74 656e 7473 206f 6620 6d65 6d79  Contents of memy
     $ examine 0x80000090
     Addr      +0   +2   +4   +6   +8   +a   +c   +e    0 2 4 6 8 a c e
     80000090  436f 6e74 656e 7473 206f 6620 0002 7f94  Contents of ....
 */

#include "../svc/svc.h"
#include "../util/reportError.h"

int cmd_examine(int argc, char *argv[])
{
	void* location;
	char string[17];
	char *eptr;
	int i,len;
	char buffer[256];

	/* Strictly accept 2 arguments */
	if(!(argc==2))
		return(INVALID_INPUT);   

	/* Convert ARGV[1]*/
	if((argv[1][0]=='0') && ((argv[1][1]=='x') || (argv[1][1]=='X'))) 
	{
		/* Hex integer constant (of arbitrary length)*/
		location = (void*) strtoul(argv[1],&eptr,16);
	}
	else if(argv[1][0]=='0')
	{
		/*Octal integer constant (of arbitrary length)*/
		location = (void*) strtoul(argv[1],&eptr,8);  
	}
	else
	{
		/*decimal integer constant (of arbitrary length)*/
		location = (void*) strtoul(argv[1],&eptr,10); 
	}

	/* The command will display 16 bytes starting at the requested address. */
	unsigned char *pointer = (unsigned char*) location;

	/* Store ASCII values in String */
	for(i=0;i<16;i++)
	{
		if(pointer[i]<=31 || pointer[i]>=127)
			string[i] = '.';
		else
			string[i] = pointer[i];
	}
	string[16] = '\0';
	
	/* Print formated string */
	svc_fputs_main(STDOUT,"Addr      +0   +2   +4   +6   +8   +a   +c   +e   0 2 4 6 8 a c e \r\n");
	len = snprintf(buffer,256,"%08x  %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %s\r\n",(unsigned int)location,pointer[0],pointer[1],pointer[2],pointer[3],pointer[4],pointer[5],pointer[6],pointer[7],pointer[8],pointer[9],pointer[10],pointer[11],pointer[12],pointer[13],pointer[14],pointer[15],string);
	if(len>=0) svc_fputs_main(STDOUT,buffer);

	return(SUCCESS);
}

