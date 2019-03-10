#include "reportError.h"
#include <stdlib.h>
#include "memory.h"

int freeAllocatedMemory(int argc,char **argv)
{
    int i = 0;
    for(i=0;i < argc ;i++)
    {
        myFree(argv[i]);
    }
    return(SUCCESS);
}
