/*
 * initializeDevicesAndMemory.c
 *
 *  Created on: Mar 19, 2017
 *      Author: Jyot Buch
 */

#include "../device/led.h"
#include "../device/pushbutton.h"
#include "../mem/mcg.h"
#include "../mem/sdram.h"
#include "../util/reportError.h"
#include "../mem/memory.h"
#include "../device/lcdc.h"
#include "../device/lcdcConsole.h"
#include "../device/deviceFcnDeclaration.h"
#include "../process/processControlUtils.h"
#include "../int/interrupt.h"
#include "../svc/svc.h"

void adc_init(void);
void consoleInit(void);
void TSI_Init(void);
void TSI_Calibrate(void);

void initalizeDevicesAndMemory(void)
{
	/* On reset,
	 * Core clock = 20.48 MHz
	 * Bus (peripheral) clock = 640 * IRC (Internal Reference Clock = 32KHz) = 20.48 MHz */

	/* Initialise all processor clocks and transition to 120 MHz full
	 * speed core clock; The DDR clock is also initialised to be
	 * running at 150 MHz */
	mcgInit();
	/* So now,
	 *  Core clock = 120 MHz
	 *  Bus (peripheral) clock = 60 MHz
	 *  FlexBus clock = 40 MHz
	 *  FLASH clock = 20 MHz
	 *  DDR clock = 150 MHz */

	/* The processor clocks must be initialised by calling mcgInit
	 * before sdramInit is called */
	sdramInit();

	setInterruptPriorities();

	/* We may initialise any register if we want
	 * Notice that from ARM instruction set, when we call a function, register r0 is first parameter 
	 * r1 is second parameter 
	 * r2 is third parameter 
	 * r3 is fourth parameter 
	 * 
	 * When function needs to return a value, the value is returned from r0.
	 * 
	 * So, if our function is not valid, still it will get a r0 as some number.
	 */
	__asm("ldr r0,=73");

	/* For Example, if we issue __asm("svc #100"); which is not in our SVC handler then 
	 * it will return 73.
	 * Issue the SVC (Supervisor Call) instruction (See A7.7.175 on page A7-503 of the
	 * ARM®v7-M Architecture Reference Manual, ARM DDI 0403Derrata 2010_Q3 (ID100710)) */
	/* This is a way to invoke SVC call item number 4 For reference only */

	/* Allocate global memory : Initialise the main memory block of size MAX_MEMORY_SIZE  */
	initializeMainMemory();

	/* Initialise all of the LEDs */
	ledInitAll();

	/* Initialise the pushbuttons */
	pushbuttonInitAll();

	/* ADC init */
	adc_init();

	/* LCDC init() */
	lcdcInit();
	consoleInit();

	/* Capacitive Pads Init and calibrate */
	TSI_Init();
	TSI_Calibrate();

	/* ei(); interrepts are default on when reset */
}

/* Memory Layout 
 * 
 * 
 * --------------------------------------------------------
 * |		SDRAM_START									  |
 * |													  |
 * |													  |  <-- Used for LCDC
 * |													  |
 * |													  |
 * |													  |
 * |		(LCDC_GW_START_ADDRESS + LCDC_GW_BUFFER_SIZE) |
 * |													  |
 * |													  |
 * |													  |
 * |													  | <-- Used for myMalloc
 * |													  |
 * |								      				  |
 * |		SDRAM_END									  |
 * --------------------------------------------------------
 */

int initializeMainMemory()
{
	/* Allocate an appropriately sized region of memory and it should return a
	 * pointer to (i.e., the address of) the first byte of that region */

	/* Allocate the max memory and return a blobal pointer to that memory */
	head = (void *)(LCDC_GW_START_ADDRESS + LCDC_GW_BUFFER_SIZE); /* malloc(MAX_MEMORY_SIZE*sizeof(char)); */
	/* sizeof(char) == 1 byte, in this case 1000000*1 byte == 1 MB */

	/* If could not allocate the memory then return error code */
	/*if(!head)
		return(MEMORY_ALLOCATION_FAILED);*/

	/*Pointing to the main block of memory which is initially free (no storage allocation yet)*/
	mainList=head;

	/* Initialising the block of memory */
	initialize();
	return(SUCCESS);
}


void initialize()
{
	/* Initialising the block of memory; all blocks are free */
	mainList->size = MAX_MEMORY_SIZE-sizeof(struct block);
	mainList->isFree = true;
	mainList->pid=FREE_PID_DEFAULT;
	mainList->next=NULL;
}


int freeAllocatedMemory(int argc,char **argv)
{
	int i = 0;
	errorCode status = SUCCESS;
	
	for(i=0; i < argc ;i++)
	{
		status = svc_myFreeErrorCode(argv[i]);
		if(status!= MEMORY_FREE_SUCCESS)return(status);
	}

	return(status);
}
