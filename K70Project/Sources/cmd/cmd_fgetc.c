/*
 * cmd_fgetc.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "../util/reportError.h"
#include <stdlib.h>
#include <stdio.h>
#include "../device/uart.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

int cmd_fgetc(int argc, char *argv[])
{
    int fd,returnArg,len;
    char *eptr;
    char buffer[100];
    
    /* Strictly accept 2 arguments */
    if(!(argc==2))
        return(INVALID_INPUT);
    
    /* argv[0] = "fgetc"
     * argv[1] = 0   //file Descriptor, 0 means get character from stdin
     */
    
    /* Convert char to integer */
    fd = strtol(argv[1],&eptr,10);
    
    returnArg = svc_fgetc_main(fd);
    
    if(returnArg>=SUCCESS)
    {
        len = snprintf(buffer,100,"Returned value is: %d.\r\nCharacter: %c \r\nUnsigned Int: %4u \r\n",returnArg,(char)returnArg,(unsigned int)returnArg);
        if(len>=0) svc_fputs_main(STDOUT,buffer);
        returnArg = SUCCESS;
    }
    
    return(returnArg);
}
