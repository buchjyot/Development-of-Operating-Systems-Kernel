/*
 * setIntrruptPriorities.c
 *
 *  Created on: Apr 15, 2017
 *      Author: Jyot Buch
 */

#include "../int/PDB0.h"
#include "../svc/svc.h"
#include "../int/systemTickTimer.h"

void setInterruptPriorities()
{
	/* Set the SVC handler priority for supervisor calls, We have range from 0 to 15 but we are setting it to middle.*
	 * Hence this will be interruptible by 0 to 6 but not interruptible by 7 to 15. 
	 * If we need to respond quickly that means those interrupts are designed to be higher priority and lower number 
	 * i.e. between 0 to 6*/
	svcInit_SetSVCPriority(15);	

	/* Perform all the necessary initialisations start a timer and set flex timer priority to 14 */
	intFlexTimerInit();

	/* NOTE: 
	 * 
	 * uartSetup() sets the UART interrupt priority to 13.
	 * 
	 * PDB0 interrupt priority is set to 14 which is same as FlexTimer Priority.
	 * 
	 * Lowest priority (15): SVC instruction
	 * Second lowest priority (14): Quantum interrupt and pendSV
	 * Higher priorities (13): UART 
	 * hardware interrupts other than
	 * the quantum interrupt all have priorities higher than the quantum
	 * interrupt
	 * Priority level 12 PDB0 and Flextimer
	 */

	/* PDB0 Timer init PDBTimerOneShot
	 */
	PDB0Init(0, PDBTimerOneShot);

	/* Sets the systick timer priority to 14*/
	sysTickTimerInit();
	
	/* Sets the pendSV priority to 14*/
	pendSVInit();
}
