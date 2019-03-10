#ifndef _PROCESS_CONTROL_UTILS_H
#define _PROCESS_CONTROL_UTILS_H

/* Single PCB (Process Control Block) struct that contains a
 * place-holder PID (Process ID) number.*/
struct PCB
{
    int PID;
};

/* The PCB should be pointed to by a global file-scope variable named,
 * currentPCB */
struct PCB currentPCB = {0};

/* Function declrations */
unsigned int getCurrentPID(void);

#endif /* ifndef _PROCESS_CONTROL_UTILS_H */
