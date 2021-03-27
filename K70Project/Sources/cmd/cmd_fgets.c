/*
 * cmd_fgets.c
 *
 *  Created on: Apr 8, 2017
 *      Author: Jyot Buch
 */

/*
 * fputs.c
 *
 *  Created on: Apr 8, 2017
 *      Author: Jyot Buch
 */

#include "../util/util.h"
#include "../util/reportError.h"
#include <stdlib.h>
#include "../svc/svc.h"

int cmd_fgets(int argc, char *argv[])
{
	int fd;
	char *eptr;

	/* Accept 3 or more arguments */
	if(argc!=2)
		return(INVALID_INPUT);

	/* argv[0] = "fgets"
	 * argv[1] = 0   //file Descriptor 
	 */
	
	/* Convert char to integer */
	fd = strtol(argv[1],&eptr,10);
	
	/* Get strings from UART STDIN is not allowed in fgets command */
	if(fd==0)
		return(FGETS_ONLY_FOR_FILES);

	return(svc_fgets_main(fd));
}

