/* cmd_free The free command accepts a single argument which is the address
 * of a region of memory previously allocated using malloc and then
 * call free to deallocate that region of memory.  The address
 * is to be specified either as a decimal integer constant (of
 * arbitrary length) or as a hexadecimal number (indicated by a prefix of
 * 0x followed by an arbitrary length hexadecimal constant).  The
 * alphabetic hexadecimal digits are to be specified in either
 * upper or lower case.  After the call returns, a success message is outputed to stdout.*/

#include "../svc/svc.h"
#include "../util/reportError.h"
#include "../util/util.h"

int cmd_freeerrorcode(int argc, char *argv[])
{
    unsigned long address;
    char *eptr;
    errorCode RetCode;
    
    /* Strictly accept only 1 arguments */
    if(!(argc==2))
        return(INVALID_INPUT);
    
    /* Make sure that specified address is in hex or decimal */
    if((argv[1][0]=='0') && ((argv[1][1]=='x') || (argv[1][1]=='X')))
    {
        address = strtoul(argv[1],&eptr,16);
        RetCode = svc_myFreeErrorCode((void *) address);
    }
    else /* if the specified address is in decimal number */
    {
        address = strtoul(argv[1],&eptr,10);
        RetCode = svc_myFreeErrorCode((void *) address);
    }
    return(RetCode);
}

