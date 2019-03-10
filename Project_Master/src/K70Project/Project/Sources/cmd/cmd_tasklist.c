/*
 * cmd_tasklist.c
 *
 *  Created on: May 7, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include "../process/processControlUtils.h"

int cmd_tasklist(int argc, char **argv)
{
	if(argc!=1)
		return(INVALID_INPUT);

	/* Tasklist function call*/
	showTaskList();	

	return(SUCCESS);
}
