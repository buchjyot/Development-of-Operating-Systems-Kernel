/* cmd_malloc calls the myMalloc API to allocate the requested memory
 *
 */

#include "memory.h"
#include "util.h"
#include <stdlib.h>
char *ptr;

int cmd_malloc(int argc, char *argv[])
{
    /* Redefine the requested size */
    unsigned int size;
    char *eptr;
    
    /* Strictly accept only 1 arguments */
    if(!(argc==2))
        return(INVALID_INPUT);
    
    /* If size is in hexadecimal then convert them to decimal, If size is
     * in decimal(ascii) then convert them to decimal(to the base 10) */
    if((argv[1][0]=='0') && ((argv[1][1]=='x') || (argv[1][1]=='X')))
        size = strtol(argv[1],&eptr,16);
    else
        size = strtoul(argv[1],&eptr,10); /* or we can use atoi() function from stdlib.h*/
    
    /* Call myMalloc command to execute the request for allocation of memory */
    /* NOTE: size is in Bytes */
    ptr = myMalloc(size*sizeof(char));
    
    /* Make sure that the second argument is hex or integer size */
    if(NULL == ptr)
        return(MEMORY_ALLOCATION_FAILED);
    
    fprintf(stdout,"Storage allocated at: %p \n",ptr);
    
    /* Since allocation is successfull return SUCCESS*/
    return(SUCCESS);
}
