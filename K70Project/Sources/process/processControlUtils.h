#ifndef _PROCESS_CONTROL_UTILS_H
#define _PROCESS_CONTROL_UTILS_H

#include <stdint.h>
#include "../util/reportError.h"
#include "../util/util.h"
#include "../device/deviceEnums.h"

#define TOTAL_NUMBER_OF_OPEN_STREAMS 20
typedef uint32_t pid_t;

typedef enum 
{
	READY_TO_RUN = 0,
	RUNNING = 1,
	BLOCKED = 2,
	TO_BE_KILLED = 3 /* This can be also refereed to as Zombie state */
}status;

struct PCB
{
	/* Name of the Process */
	char processName[20];

	/* Single PCB (Process Control Block) struct that contains a
	 * place-holder PID (Process ID) number.*/
	pid_t PID;

	/* Status of the process */
	status process_status;

	/* Stack Structure to keep track of Stack SP, size and the lowest Addr*/
	struct stack
	{		
		uint32_t SP;/* The Stack pointer for the process points to the top of the stack*/		
		uint32_t stackSize; /* Size of the stack */		
		uint32_t lowestAddr; /* Lowest Address of the stack */
	}stackData;

	/* CPU_TIME */
	uint64_t CPU_TIME;
	
	/* Mem Usage*/
	uint64_t memUsage;

	/* File Descriptor is an index to openStreams struct */
	struct stream
	{
		bool isFree;
		char name[256];  /* Name of the stream */
		int majorDeviceID; /* Major Device Category */
		int minorDeviceID; /* Minor Device Category */
		void *minorStruct; /* Minor device Structure */	
		char *cursor; 	 /* Cursor Location for reading the byte */
		struct file_node *currentFileNode; /* file book keeping for file node */
		char mode[3]; /* mode at which file is asked to be opened */

	}openStreams[TOTAL_NUMBER_OF_OPEN_STREAMS]; /* Array that has a list of pointers to any device that is open in a process (fixed numbers of devices which can be open by multiple processes) */

	/* Pointer to the next PCB */
	struct PCB* next;

};

/* The PCB should be pointed to by a global variable named currentPCB */
struct PCB *currentPCB;

/* Head of the PCB */
struct PCB *headPCB;

/* Function Prototypes  */
/* Returns the pointer to the current PCB */
void* SVCgetCurrentPCBImpl(void);

/* Returns the free file descriptor from the PCB */
int SVCgetFreeFileDiscriptorImpl(void);

/* getCurrentPID returns pid of current process */
pid_t SVCgetCurrentPIDImpl(void);		

/* spawn returns indication of success */
/* main is the function to be run by the newly created process */
/* argc is the argc to be passed to main */

/* argv is the argv to be passed to main;
 * NOTE: the argv array and the strings
 * pointed to by argv need to be copied
 * into newly allocated memory that will be
 * owned by the new process */

struct spawnStruct
{
	char *process_name;
	int (*fcnToRunByTheNewProcess)(int argc, char *argv[]);
	int argc;
	char **argv;
	uint32_t stackSize;
	pid_t *spawnedPidPtr;
};

/* stackSize is the size of the stack to be allocated for the new process */
/* sets spawnedPid to pid of spawned process */
int SVCspawnImpl(void*);

/* yields remaining quantum */
void SVCyieldImpl(void);	

/* sets the current process to blocked state */
void SVCblockImpl(void);		

/* sets the targetPid process to blocked state */
/* returns indication of success */
int SVCblockPidImpl(pid_t targetPid);

/* sets the targetPid process to ready state */
/* returns indication of success */
int SVCwakeImpl(pid_t targetPid);	

int SVCkillImpl(pid_t targetPid);	
/* prematurely terminates the targetPid process */
/* returns indication of success */

void SVCwaitImpl(pid_t targetPid);
/* waits for the targetPid process to end execution (naturally or prematurely) */

pid_t getAvailablePIDNumber(void);
void addThisPCBtoGlobalList(struct PCB *);
void unlinkPCBAddrFromGlobalList(struct PCB *);
void createCustomStack(struct spawnStruct*,struct stack*);
unsigned int getTotalNumberOfProcesses(void);
void* getPCBbyPID(uint32_t);
void closeAllOpenStreamsForThisPCB(struct PCB*);

void setPCBEnvForPID(uint32_t);
void unsetPCBEnvForPID(void);
bool queryPCBEnvForPID(void);

bool queryFirstProcessFlag(void);
bool processExists(uint32_t);

void showTaskList(void);
uint64_t getMemoryOccupiedByPID(uint32_t);


#endif /* ifndef _PROCESS_CONTROL_UTILS_H */
