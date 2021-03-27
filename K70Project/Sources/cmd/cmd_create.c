/*
 * cmd_create.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include <stdio.h>
#include "../device/deviceEnums.h"
#include "../device/deviceFcnDeclaration.h"
#include "../svc/svc.h"

int cmd_create(int argc, char *argv[])
{
	/* Strictly accept 2 arguments */
	if(!(argc==2))
		return(INVALID_INPUT);

	/* argv[0] = "create"
	 * argv[1] =  "filename" 
	 */

	int returnArg = svc_create(argv[1]);
	return(returnArg);
}
