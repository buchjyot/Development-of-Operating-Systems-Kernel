/**
 * intPDBOneShotTest.c
 * Programmable Delay Block (PDB) one shot interrupt test program
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
 * that a pointer to the PDB0Isr function is in the vector table at vector 88 (0x0000_0160)
 * for Programmable Data Block (PDB).
 *
 * The following declaration needs to inserted earlier in the file:
 *   extern void PDB0Isr(void);
 *
 * If using the GCC Toolchain, the vector table is named "InterruptVector", and the line:
 *   Default_Handler,		(comment delimiters removed) Vector 88: PDB
 * needs to be changed to:
 *   PDB0Isr,			(comment delimiters removed) Vector 88: PDB
 *
 * If using the Freescale Toolchain, the vector table is named "__vect_table", and the line:
 *   (tIsrFunc)UNASSIGNED_ISR,    (comment delimiters removed) 88 (0x00000160) (prior: -)
 * needs to be changed to:
 *   (tIsrFunc)PDB0Isr,		  (comment delimiters removed) 88 (0x00000160) (prior: -)
 */

#include <stdio.h>
#include "SVCPDBUserTimer.h"
#include "PDB0.h"

/**
 * this routine contains all actions to be performed when a PDBTimer 0
 * interrupt occurs.
 *
 * alternate turning the orange LED on and off with each interrupt.
 */
void PDB0Action(void) 
{
	user_fcn.functionp();
}

void SVCsetPDBTimerImpl(int time,void* myFcn,int continousOrOneShot)
{
	/* convert the time to count and set the count for the timer and start the timer*/
	uint16_t count = time*11718/1000; /* assuming time provided in milli seconds */
	
	/* 
	 * fpheripheral = 60 MHz
	 * 
	 * fprescalar = fpheripheral / Prescalor = 60 MHz / 128 = 468750 Hz
	 * 
	 * fpdb = fprescalar / Mult = 468750 / 40 = 11718.75 Hz
	 * 
	 * So PDB0 interrupt occurs at every 1/11718.75 seconds
	 * 
	 * So providing count of 11718 should give interrupt at every one second.
	 */

	if(time==0)
	{ 
		/* Stop the timer */
		PDB0Stop();
		
		/* Set the User supplied function to NULL value.*/
		user_fcn.functionp = myFcn;
	}
	else
	{
		/* set interval to 16384 1/16384 second periods or 1 second notice that continousOrOneShot is the mode specified by user*/
		PDB0Init(count, continousOrOneShot);

		/* Set the User supplied function to call*/
		user_fcn.functionp = myFcn;

		/* Start the timer */
		PDB0Start();
	}
}
