/*
 * cmd_deposit.c
 *
 *  Created on: Apr 15, 2017
 *      Author: Jyot Buch
 */

#include "../svc/svc.h"
#include "../util/reportError.h"

int cmd_deposit(int argc, char *argv[])
{
	/* Strictly accept >=3 arguments */
	if(!(argc>=3))
		return(INVALID_INPUT);   

	void *location;
	char value; /* One byte value */
	char *eptr;
	int i,j;

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


	unsigned char *pointer = (unsigned char*) location;

	j = 0;

	for(i=2;i<=argc-1;i++)
	{
		/* Convert ARGV[2]*/
		if((argv[i][0]=='0') && ((argv[i][1]=='x') || (argv[i][1]=='X'))) 
		{
			/* Hex integer constant */
			value = strtoul(argv[i],&eptr,16);
		}
		else if(argv[i][0]=='0')
		{
			/*Octal integer constant */ 
			value = strtoul(argv[i],&eptr,8);
		}
		else
		{
			/*decimal integer constant */ 
			value = strtoul(argv[i],&eptr,10);
		}

		pointer[j] = value;
		j++;
	}


	return(SUCCESS);
}
