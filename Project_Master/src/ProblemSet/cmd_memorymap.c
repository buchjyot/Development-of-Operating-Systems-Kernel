/* cmd_memoryMap */

#include "memory.h"

int cmd_memorymap(int argc, char *argv[]) /* calls myMemoryMap */
{
    if(argc!=1)
        return(INVALID_INPUT);
    
    /* Print Memory Map by calling myMemoryMap function */
    myMemoryMap();
    return(SUCCESS);
}
