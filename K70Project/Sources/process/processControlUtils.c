/* This is a general function which contains the defination of
 * process control block and their functions */

#include "processControlUtils.h"
#include <stdio.h>
#include "../device/uart.h"
#include "../svc/svc.h"

struct processStatusTable
{
	char* statusString;

}statusTable[] = {
		{"READY_TO_RUN"},
		{"RUNNING"},
		{"BLOCKED"},
		{"TO_BE_KILLED"}
};

unsigned int NUBER_OF_TOTAL_PROCESSES = 0;
bool PCB_ENV_FOR_PID = 0;
unsigned int PID_VALUE_FOR_PCB_ENV = 0;

/* Function named getCurrentPID that returns the PID contained
 * in the PCB of the current process (i.e., in the PCB pointed to by
 * currentPCB).*/

pid_t SVCgetCurrentPIDImpl()
{
	if(PCB_ENV_FOR_PID)
		return(PID_VALUE_FOR_PCB_ENV);
	else
		return(currentPCB->PID);
}

void* SVCgetCurrentPCBImpl()
{
	/* Currently this function initialises a global PCB
	 * Later in this course we will have PCB for each process.
	 */
	if(PCB_ENV_FOR_PID)
		return(getPCBbyPID(PID_VALUE_FOR_PCB_ENV));
	else
		return(currentPCB);	
}

int SVCgetFreeFileDiscriptorImpl(void)
{
	struct PCB* myPCB = (struct PCB* )SVCgetCurrentPCBImpl();
	int i;

	for(i = 0 ; i < TOTAL_NUMBER_OF_OPEN_STREAMS; i++)
	{
		/* If you find a any openStream to be free then return the index */
		if(myPCB->openStreams[i].isFree)
			return i;			
	}

	/* You reached here because all the possible openStreams are in use*/
	return(MAX_OPEN_STREMS_LIMIT_REACHED);
}

int SVCspawnImpl(void* myStruct)
{
	/* In order to schedule processes, Our 
	 * operating system should maintain a process list with a process
	 * control block (PCB) for each process.*/

	int i = 0;
	struct spawnStruct *requestedProcessConfig = (struct spawnStruct*)myStruct;

	/* all the new PCBs are owned by the Shell or Kernel with PID = 1 */
	setMallocForOSEnv();

	/* Create a brand new PCB */
	struct PCB *newPCB = (struct PCB*)SVCmyMallocImpl(sizeof(struct PCB));

	if(!newPCB)
		return(UNABLE_TO_SPAWN_A_PROCESS);

	/* Initialize the PCB */
	newPCB->PID = getAvailablePIDNumber();	 /* Change the pid, for the first process it will be same as OCCUPIED_PID_BY_OS_DEFAULT*/
	requestedProcessConfig->spawnedPidPtr = &newPCB->PID;
	
	/* Initialize all open streams to be free*/
	while(i < TOTAL_NUMBER_OF_OPEN_STREAMS)
	{
		/* Have all the open Streams to be free */
		newPCB->openStreams[i].isFree = true;		
		i++;
	}

	/* Let the stackpointer point to the function to run */
	createCustomStack(requestedProcessConfig,&(newPCB->stackData));

	/* Make the new process to be ready to run */
	newPCB->process_status = READY_TO_RUN;
	
	/* Copy the name of the process */
	strcpy_fcn(newPCB->processName,requestedProcessConfig->process_name);

	/* Initialize the CPU time */
	newPCB->CPU_TIME = 0;

	/* Memory occupied by this process, in other words memory occupied by PID of this process*/
	newPCB->memUsage = 0;

	/* After configuring the process, add the new PCB to Global list */
	/* For the first process this also makes currentPCB to point to the headPCB*/
	addThisPCBtoGlobalList(newPCB);

	/* all the new PCBs are owned by the Shell or Kernel*/
	unsetMallocForOSEnv();

	return(SUCCESS);
}

/* yields remaining quantum */
void SVCyieldImpl(void)
{
	/* Set the pendSV bit of the ICSR Register*/
	SCB_ICSR |= SCB_ICSR_PENDSVSET_MASK;
}

/* sets the current process to blocked state */
void SVCblockImpl(void)
{
	currentPCB->process_status = BLOCKED;
}

/* sets the targetPid process to blocked state */
/* returns indication of success */
int SVCblockPidImpl(pid_t targetPid)
{
	struct PCB *targetPCB = (struct PCB*) getPCBbyPID(targetPid);
	targetPCB->process_status = BLOCKED;
	return(SUCCESS);
}

/* sets the targetPid process to ready state */
/* returns indication of success */
int SVCwakeImpl(pid_t targetPid)
{
	struct PCB *targetPCB = (struct PCB*) getPCBbyPID(targetPid);
	targetPCB->process_status = READY_TO_RUN;
	return(SUCCESS);
}

/* prematurely terminates the targetPid process */
/* returns indication of success */
int SVCkillImpl(pid_t targetPid)
{

	/* All dynamically-allocated (malloc'ed) storage owned 
	 * by the process that is ending needs to be freed.*/
	freeMemoryForPID(targetPid);

	/* Get the address of the PCB block for the targetPid. */
	void* targetPCBaddr = getPCBbyPID(targetPid);

	/* When a process ends (naturally or when killed), any open streams
     need to be closed and the storage used for its PCB and for its
     stack must be reclaimed. */
	closeAllOpenStreamsForThisPCB(targetPCBaddr);

	/* Unlink the PCB address from global list*/
	unlinkPCBAddrFromGlobalList(targetPCBaddr);

	/* Now free the PCB memory which was occupied by OS*/
	setMallocForOSEnv();
	int returnCode = SVCmyFreeErrorCodeImpl(targetPCBaddr);
	unsetMallocForOSEnv();

	return(returnCode);
}

/* Waits for the targetPid process to end execution (naturally or prematurely) */
void SVCwaitImpl(pid_t targetPid)
{
	struct PCB *targetPCB = (struct PCB *)getPCBbyPID(targetPid);
	
	while(targetPCB->process_status== RUNNING)
	{}
	
}

pid_t getAvailablePIDNumber()
{
	pid_t i;
	struct PCB *currentPCBStruct = (struct PCB *) SVCgetCurrentPCBImpl();
	struct PCB *temp;

	if(queryFirstProcessFlag())
		return(OCCUPIED_PID_BY_OS_DEFAULT);

	/* Continuously loop over all the PID value and return the next available PID */
	for(i=1;i<=UINT32_MAX;i++)
	{		
		temp = currentPCBStruct;
		temp = temp->next;	/* offset of one PCB */

		while(1)
		{
			/* if you find the i which is currently being used then just break to go over next i*/
			if(i == temp->PID)
			{
				break;
			}
			else
			{
				/* if i != temp->pid then simply go to next PCB and loopover, until you reach the currentPCB */
				temp = temp->next;	

				/* if you are at currentPCB then, first check if current PCB have a same pid as i ? 
				 * if not then return i, because thats the next incremental PCB that you will
				 * assign to the new process*/
				if(temp == currentPCBStruct)
				{
					if(i == temp->PID)
					{
						break;
					}
					else
					{
						return(i);
					}
				}
			}
		}
	}
}

void updateCPUTime(void* pcbStruct)
{
	struct PCB* incomingPCB = (struct PCB* )pcbStruct;

	/* Update 64 bit time period */
	incomingPCB->CPU_TIME = incomingPCB->CPU_TIME + (SYST_RVR - SYST_CVR);
}

void addThisPCBtoGlobalList(struct PCB *newPCBStruct)
{

	/* When you call first time it will have headPCB = NULL */
	if(!headPCB)
	{
		headPCB = newPCBStruct;
		headPCB->next = headPCB;		
		currentPCB = headPCB;
	}
	else
	{
		struct PCB *tempPCB = headPCB;
		int i;

		for(i=1;i<NUBER_OF_TOTAL_PROCESSES;i++)
		{
			tempPCB = tempPCB -> next;
		}

		tempPCB -> next = newPCBStruct;
		newPCBStruct->next = headPCB;
	}

	/* Since we created a new PCB increment the NUBER_OF_TOTAL_PROCESSES count */
	NUBER_OF_TOTAL_PROCESSES++;
}

void unlinkPCBAddrFromGlobalList(struct PCB *pcbToUnlink)
{
	int i;
	struct PCB *tempPCB = headPCB;
	struct PCB *previous = NULL;

	for(i=1;i<NUBER_OF_TOTAL_PROCESSES;i++)
	{
		if(tempPCB == pcbToUnlink)
		{
			/* If this is the rootProcess, this operation is not allowed so just return */
			if(tempPCB==headPCB)
				return; /* previous will be NULL here since you just entered */
			else
			{
				previous->next = tempPCB->next;
				return; /* You are now done unlinking the tempPCB or the pcbToUnlink(in this case) */
			}
		}
		else
		{
			previous = tempPCB;
			tempPCB = tempPCB -> next;
		}
	}

	/* Since we unlinked a process decrement the NUBER_OF_TOTAL_PROCESSES count */
	NUBER_OF_TOTAL_PROCESSES--;
}

unsigned int getTotalNumberOfProcesses()
{
	return(NUBER_OF_TOTAL_PROCESSES);
}

void createCustomStack(struct spawnStruct *processConfig,struct stack *newStack)
{
	/* Creates a custom stack and returns its stack pointer */
	uint32_t topOftheStack = (uint32_t)svc_myMalloc(processConfig->stackSize);
	
	/* Go to bottom of the stack */
	uint32_t botOfStack = topOftheStack + processConfig->stackSize;
	
	/* ------------------------
	 * topOftheStack (returned from Malloc)
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * ________________________
	 * 
	 * START OF STACK
	 * 
	 * 
	 * 
	 * 
	 * END OF STACK
	 * _______________________
	 * botOfStack
	 * ------------------------
	 */

	/* Start writing registers */
	
	
	/* Book-Keeping of stack */
	newStack->SP = 1;
	newStack->lowestAddr = botOfStack;
	newStack->stackSize = processConfig->stackSize;

}

void closeAllOpenStreamsForThisPCB(struct PCB* targetPcb)
{
	int i = 0;
	while(i < TOTAL_NUMBER_OF_OPEN_STREAMS)
	{

		if(targetPcb->openStreams[i].isFree == false)
		{
			/* Set the env for PID so that now SVCgetCurrentPCBImpl will return the PCB of this PID and not the currentPCB */
			setPCBEnvForPID(targetPcb->PID);
			SVCfclose_mainImpl(i);
			unsetPCBEnvForPID();
		}
		i++;
	}
}

void setPCBEnvForPID(uint32_t targetPIDVal)
{
	PCB_ENV_FOR_PID = true;
	PID_VALUE_FOR_PCB_ENV = targetPIDVal;
}

void unsetPCBEnvForPID()
{
	PCB_ENV_FOR_PID = false;
	PID_VALUE_FOR_PCB_ENV = false;
}

bool queryPCBEnvForPID()
{
	return(PCB_ENV_FOR_PID);
}

void* getPCBbyPID(uint32_t inputPID)
{

	struct PCB *tempPCB = headPCB;
	int i;

	for(i=1;i<NUBER_OF_TOTAL_PROCESSES;i++)
	{
		if(tempPCB->PID == inputPID)
			return((void*)tempPCB);
		else
			tempPCB = tempPCB -> next;
	}

	/* Retrun NULL so that it knows we could not find the PCB with the provided PID */
	return(NULL);
}

void updateMemoryUsageforThePCB(struct PCB *inputPCB)
{
	inputPCB->memUsage = getMemoryOccupiedByPID(inputPCB->PID);
}

void showTaskList()
{
	svc_fputs_main(STDOUT,"====================================\r\n");
	svc_fputs_main(STDOUT,"Task List\r\n");
	svc_fputs_main(STDOUT,"====================================\r\n");

	struct PCB *tempPCB = headPCB;
	struct PCB *previousPCB = NULL;
	char buffer[200];
	int len,i;

	for(i=1;i<=NUBER_OF_TOTAL_PROCESSES;i++)
	{
		/* Update values so we show the accurate values*/
		updateMemoryUsageforThePCB(tempPCB);

		/* only for Demo */
		tempPCB->CPU_TIME = svc_gettimeofday();

		len = snprintf(buffer,200,"Task Name: %s \r\nPID: %u \r\nCPU_TIME: %llu \r\nMemory Usage: %llu Bytes\r\nCurrent Status: %s\r\n",tempPCB->processName,(unsigned int)tempPCB->PID,tempPCB->CPU_TIME,tempPCB->memUsage,statusTable[tempPCB->process_status].statusString);
		if(len>=0) svc_fputs_main(STDOUT,buffer);

		previousPCB = tempPCB;
		tempPCB = tempPCB->next;

		if(tempPCB!=previousPCB)
			svc_fputs_main(STDOUT,"------------------------------------\r\n");
	}

	svc_fputs_main(STDOUT,"====================================\r\n");

}

bool processExists(uint32_t queryPid)
{
	void* addr = getPCBbyPID(queryPid);
	
	/* If address is not null that means process does exists so return true */
	if(addr)
		return(true);
	else
		return(false);
}
