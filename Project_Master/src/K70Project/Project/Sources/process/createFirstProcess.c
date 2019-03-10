/*
 * createFirstProcess.c
 *
 *  Created on: Apr 30, 2017
 *      Author: Jyot Buch
 */

#include "../int/systemTickTimer.h"
#include "../operatingSystem.h"
#include "../device/uart.h"
#include "../util/reportError.h"

int fileSystemInit(void);

int createFirstProcess(int mode)
{
	/* Capture the errorCode */
	errorCode rootStatus = SUCCESS;
	
	/* Setup Process 1 parameters : Create a Process-1 PCB and stack */
	setMallocForOSEnv();
	struct spawnStruct *inputStruct = (struct spawnStruct*) svc_myMalloc(sizeof(struct spawnStruct));
	unsetMallocForOSEnv();

	inputStruct->process_name = "Shell";
	inputStruct->fcnToRunByTheNewProcess = &shell;
	inputStruct->argc = 0;
	inputStruct->argv = NULL;
	inputStruct->stackSize = 512;	/* In Bytes */

	/* SPAWN a brand new first process */
	rootStatus = svc_spawn(inputStruct);
	if(rootStatus!=SUCCESS)svc_reportError(rootStatus);

	/* Free the inputStruct, since now we do not need it, currentPCB is pointing to the correct location */
	setMallocForOSEnv();
	rootStatus = svc_myFreeErrorCode((void*)inputStruct);
	if(rootStatus!=SUCCESS)svc_reportError(rootStatus);
	unsetMallocForOSEnv();

	/* Setup UART so that any reportError code succeeds */
	uartSetup();

	/* Pre-Initialise/Open default stream in to PCB for the process-1 Only*/
	SVCfopen_mainImpl("stdin","r");
	SVCfopen_mainImpl("stdout","w");
	SVCfopen_mainImpl("stderr","w");

	/* Initialize File System */
	rootStatus = fileSystemInit();
	if(rootStatus!=SUCCESS) svc_reportError(rootStatus);

	/* Validate User */
	rootStatus = validateUser(true);
	svc_reportError(rootStatus); 

	if(rootStatus == LOGIN_SUCCESSFULL)
	{
		switch(mode)
		{

		case SINGLE_PROCESS_MODE:
			/* Before calling the shell make a call to privUnprivileged so that now all the actions are unprivileged*/
			currentPCB->process_status = RUNNING;
			privUnprivileged();
			rootStatus = shell(0,NULL);
			break;

		case MULTI_PROGRAMMING_MODE:		
			/* systick Timer Interrupt Init() */
			sysTickTimerInit();

			/* Start systick Timer interrupt (Quantam interrupt) to switch processes */
			sysTickTimerStart();

			/* Before calling the shell(process-1) make a call to privUnprivileged so that now all the actions are unprivileged */
			privUnprivileged();

			/* Go to infinite loop */
			while(true){}; /* Runs infinite loop until Systick interrupt occurs */

			/* NOTE: When there will be a first systick interrupt hit, it will call shell() and we will never 
			 * return to this function or the main function until we restart. 
			 * 
			 * Notice that the first and the last process will be our shell(), so according to the notion of 
			 * lastProcessDonotGoAway it will be running till the end of the OS execution or restart.
			 * 
			 * That's the beauty of switching processes !
			 */
			break;
		}
	}

	return(rootStatus);
}
