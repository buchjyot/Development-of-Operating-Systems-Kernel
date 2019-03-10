/* cmd_malloc calls the myMalloc API to allocate the requested memory
 *
 */

#include "../util/util.h"
#include <stdlib.h>
#include <stdio.h>
#include "../util/reportError.h"
#include "../svc/svc.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"
char *ptr;

int cmd_malloc(int argc, char *argv[])
{
    /* Redefine the requested size */
    int size;
    char *eptr;
    char buffer[50];
    int len;
    
    /* Strictly accept only 1 arguments */
    if(!(argc==2))
        return(INVALID_INPUT);
    
    /* If size is in hexa-decimal then convert them to decimal, If size is
     * in decimal(ASCII) then convert them to decimal(to the base 10) */
    if((argv[1][0]=='0') && ((argv[1][1]=='x') || (argv[1][1]=='X')))
        size = strtoul(argv[1],&eptr,16);
    else
        size = strtoul(argv[1],&eptr,10); /* or we can use atoi() function from stdlib.h*/
    
    /* strtoul returns the unsigned long value*/
    if(size<=0)
    	return(CANNOT_ALLOCATE_SIZE_0_OR_NEGETIVE);
    
    /* NOTE: UINT_MAX = ULONG_MAX = 4294967295, Sizes more then this are not supported by this malloc*/
    	
    /* Call myMalloc command to execute the request for allocation of memory */
    /* NOTE: size is in Bytes */
    ptr = svc_myMalloc((unsigned int)size*sizeof(char));
    
    /* Make sure that the second argument is hex or integer size */
    if(NULL == ptr)
        return(MEMORY_ALLOCATION_FAILED);
    
    /*fprintf(stdout,"Storage allocated at: %p \n",ptr);*/
    
    len = snprintf(buffer,50,"Storage allocated at: %p \r\n",ptr);
    if(len>=0) svc_fputs_main(STDOUT,buffer);
    
    /* Since allocation is successfull return SUCCESS*/
    return(SUCCESS);
}
