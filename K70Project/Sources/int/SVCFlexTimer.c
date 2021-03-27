/**
 * SVCFlexTimer.c
 * FlexTimer interrupt test program
 * 
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2014, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

/*
 * Important note:
 * 
 * The file Project_Settings -> Startup_Code -> kinetis_sysinit.c needs to be modified so
 * that a pointer to the flexTimer0Isr function is in the vector table at vector 78 (0x0000_0138)
 * for Flex Timer 0 (FTM0).
 * 
 * The following declaration needs to inserted earlier in the file:
 *   extern void flexTimer0Isr(void);
 *   
 * If using the GCC Toolchain, the vector table is named "InterruptVector", and the line:
 *   Default_Handler,		(comment delimiters removed) Vector 78: FTM0
 * needs to be changed to:
 *   flexTimer0Isr,		(comment delimiters removed) Vector 78: FTM0
 *   
 * If using the Freescale Toolchain, the vector table is named "__vect_table", and the line:
 *   (tIsrFunc)UNASSIGNED_ISR,    (comment delimiters removed) 78 (0x00000138) (prior: -)
 * needs to be changed to:
 *   (tIsrFunc)flexTimer0Isr,	  (comment delimiters removed) 78 (0x00000138) (prior: -)
 */

#include <stdio.h>
#include "SVCFlexTimer.h"
#include "flexTimer.h"
#include "interrupt.h"
#include "../util/reportError.h"

long long int epochCount = 0;

/**
 * perform overall initialisation.  the LCD, button states, scroll counters,
 * and timer are all set to known states and set in motion.
 */
void intFlexTimerInit(void) 
{
	/* set interval to 1 ms by setting count to 1875 */
	flexTimer0Init(1875);
}

/**
 * this routine contains all actions to be performed when a FlexTimer 0
 * interrupt occurs.
 *
 * alternate turning the orange LED on and off with each interrupt.
 */
void flexTimer0Action(void) {

	/* Increment the millisecound count i.e. epoch */
	epochCount = epochCount + 1;
}


int SVCsettimeofdayImpl(unsigned long long int input)
{
	/* Assign user specified time input to epoch time */
	epochCount = input;
	
	/* Start the timer */
	flexTimer0Start();

	return(SUCCESS);
}


unsigned long long int SVCgettimeofdayImpl()
{
	if(epochCount==0)
		return(USER_MUST_SET_THE_EPOCH_CLOCK);
	else
		return(epochCount);
}
