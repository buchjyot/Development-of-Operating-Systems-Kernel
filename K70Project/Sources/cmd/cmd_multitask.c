/*
 * cmd_multitask.c
 *
 *  Created on: May 6, 2017
 *      Author: welcome
 */

#include "../util/reportError.h"
#include "../mem/memory.h"
#include "../svc/svc.h"
#include "../cmd/commandsPrototypes.h"

int cmd_multitask(int argc, char **argv)
{
	/* Strictly accept only one argument */
	if(argc != 1)
		return(INVALID_INPUT);

	errorCode status = SUCCESS;

	/* creating three processes to accomplish the following tasks:
	 * 
	 * one per process: 
	 * 
	 * (First process)  :copying from UART2 input to the LCD display, 
	 * (Second process) :sending a message over UART2 output whenever pushbutton S2 is depressed,
	 * (Third process)  :using the supervisor call for user timer events, flash the orange LED on 
	 * 				     and off every half a second (the LED will light once a second). 
	 * 				     
	 * In all cases, any I/O operations are be performed using device independent supervisor calls 
	 * (fopen, fgetc, fputc, and fclose, or your equivalents).
	 * 
	 * The first process (the one that performs input from UART2), will terminate when control-D is entered.
	 * When the shell's multitask command determines that that process has terminated, it will kill the other two processes.
	 */


	/**********************************************************************************************************/
	setMallocForOSEnv();
	struct spawnStruct *inputStruct1 = (struct spawnStruct*) svc_myMalloc(sizeof(struct spawnStruct));
	unsetMallocForOSEnv();

	inputStruct1->process_name = "Serial2LCD";
	inputStruct1->fcnToRunByTheNewProcess = &cmd_ser2lcd;
	inputStruct1->argc = 1;
	inputStruct1->argv = NULL;
	inputStruct1->stackSize = 512;	/* In Bytes */

	/* SPAWN a process */
	svc_spawn(inputStruct1);

	/* Free the inputStruct, since now we do not need it, we have added the actual struct to the linked list */
	setMallocForOSEnv();
	status = svc_myFreeErrorCode((void*)inputStruct1);    
	unsetMallocForOSEnv();
	if(status!=MEMORY_FREE_SUCCESS)return(status);
	/**********************************************************************************************************/
	setMallocForOSEnv();
	struct spawnStruct *inputStruct2 = (struct spawnStruct*) svc_myMalloc(sizeof(struct spawnStruct));
	unsetMallocForOSEnv();

	inputStruct2->process_name = "PB2Serial";
	inputStruct2->fcnToRunByTheNewProcess = &cmd_pb2ser;
	inputStruct2->argc = 1;
	inputStruct2->argv = NULL;
	inputStruct2->stackSize = 512;	/* In Bytes */

	/* SPAWN a process */
	svc_spawn(inputStruct2);

	/* Free the inputStruct, since now we do not need it, we have added the actual struct to the linked list */
	setMallocForOSEnv();
	status = svc_myFreeErrorCode((void*)inputStruct2);
	unsetMallocForOSEnv();
	if(status!=MEMORY_FREE_SUCCESS)return(status);	
	/**********************************************************************************************************/
	setMallocForOSEnv();
	struct spawnStruct *inputStruct3 = (struct spawnStruct*) svc_myMalloc(sizeof(struct spawnStruct));
	unsetMallocForOSEnv();

	inputStruct3->process_name = "FlashLED";
	inputStruct3->fcnToRunByTheNewProcess = &cmd_flashled;
	inputStruct3->argc = 1;
	inputStruct3->argv = NULL;
	inputStruct3->stackSize = 512;	/* In Bytes */

	/* SPAWN a process */
	svc_spawn(inputStruct3);

	/* Free the inputStruct, since now we do not need it, we have added the actual struct to the linked list */
	setMallocForOSEnv();
	status = svc_myFreeErrorCode((void*)inputStruct3);
	unsetMallocForOSEnv();
	if(status!=MEMORY_FREE_SUCCESS)return(status);
	/**********************************************************************************************************/

	/* Go to infinite loop and check if process 1 goes away */
	while(processExists(*(inputStruct1->spawnedPidPtr)))
	{
		/* Do nothing; Just Wait */
	}

	/* The moment process 1 exit by pressing char CTRL+D we will reach here */
	status = svc_kill(*(inputStruct2->spawnedPidPtr));
	if(status != MEMORY_FREE_SUCCESS)return(status);

	status = svc_kill(*(inputStruct3->spawnedPidPtr));
	if(status != MEMORY_FREE_SUCCESS)return(status);

	return(SUCCESS);
}
