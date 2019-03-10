/* This is a general function which contains the defination of
 * process control block and their functions */

#include "processControlUtils.h"

/* Function named getCurrentPID that returns the PID contained
 * in the PCB of the current process (i.e., in the PCB pointed to by
 * currentPCB).*/
unsigned int getCurrentPID()
{
    return(currentPCB.PID);
}
