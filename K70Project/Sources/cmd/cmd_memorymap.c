/* cmd_memoryMap */

#include "../mem/memory.h"
#include "../svc/svc.h"

int cmd_memorymap(int argc, char *argv[]) /* calls myMemoryMap */
{
    if(argc!=1)
        return(INVALID_INPUT);
    
    /* Print Memory Map by calling myMemoryMap function */
    svc_myMemoryMap();
    return(SUCCESS);
}
