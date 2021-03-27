/*
 * systemTickTimer.c
 *
 *  Created on: May 2, 2017
 *      Author: Jyot Buch
 */

/* [2] R/W CLKSOURCE Indicates the SysTick clock source:
 * 		0 = SysTick uses the IMPLEMENTATION DEFINED external reference clock.	
 * 		1 = SysTick uses the processor clock.	
 * 		If no external clock is provided, this bit reads as 1 and ignores writes.
 * 
 * [1] R/W TICKINT Indicates whether counting to 0 causes the status of the SysTick exception to change to pending:
 * 		0 = Count to 0 does not affect the SysTick exception status.
 * 		1 = Count to 0 changes the SysTick exception status to pending.
 * 		Changing the value of the counter to 0 by writing zero to the SysTick Current
 * 		Value register to 0 never changes the status of the SysTick exception.
 * 		
 * [0] R/W ENABLE Indicates the enabled status of the SysTick counter:
 * 		0 = Counter is disabled
 * 		1 = Counter is operating.
 * 
 */

#include <derivative.h>
#include "../svc/svc.h"
#include "../int/nvic.h"


#define SysTick_IRQn -1
#define PendSV_IRQn -2
#define PENDSV_INTERRUPT_PRIORITY 14
#define SYSTICK_INTERRUPT_PRIORITY 14
int sysTickPauseCount = 0;

/* The timer consists of four registers:
• A control and status register. This configures the SysTick clock, enables the counter, enables the
SysTick interrupt, and indicates the counter status.
• A counter reload value register.This provides the wrap value for the counter.
• A counter current value register.
• A calibration value register. This indicates the preload value required for a 10ms (100Hz) system
clock.

When enabled, the timer counts down from the value in SYST_CVR, see SysTick Current Value Register,
SYST_CVR on page B3-748. When the counter reaches zero, it reloads the value in SYST_RVR on the next
clock edge, see SysTick Reload Value Register, SYST_RVR on page B3-747. It then decrements on
subsequent clocks. This reloading when the counter reaches zero is called wrapping.
When the counter transitions to zero, it sets the COUNTFLAG status bit to 1. Reading the COUNTFLAG
status bit clears it to 0.

Writing to SYST_CVR clears both the register and the COUNTFLAG status bit to zero. This causes the
SysTick logic to reload SYST_CVR from SYST_RVR on the next timer clock. A write to SYST_CVR does
not trigger the SysTick exception logic.
Reading SYST_CVR returns the value of the counter at the time the register is accessed.
Writing a value of zero to SYST_RVR disables the counter on the next wrap. The SysTick counter logic
maintains this counter value of zero after the wrap 
Setting SYST_RVR to zero has the effect of disabling the SysTick counter independently of the
counter enable bit.

• The SYST_CVR value is UNKNOWN on reset. Before enabling the SysTick counter, software must
write the required counter value to SYST_RVR, and then write to SYST_CVR. This clears
SYST_CVR to zero. When enabled, the counter reloads the value from SYST_RVR, and counts
down from that value, rather than from an arbitrary value
 */

void sysTickInit_SetPriority(unsigned char priority) {
	if(priority > MaxPriority)
		return;

	SCB_SHPR3 = (SCB_SHPR3 & ~SCB_SHPR3_PRI_15_MASK) |
			SCB_SHPR3_PRI_15(priority << PriorityShift);

	/* Enable interrupts from Systick and set its interrupt priority in the vector table*/
	NVICEnableIRQ(SysTick_IRQn, priority);
}

/* Set the priority of the PendSV interrupt 
 * to the same value as the SysTick interrupt
 * [31:24] PRI_15 Priority of system handler 15, SysTick
 * [23:16] PRI_14 Priority of system handler 14, PendSV 
 */
void pendSVInit_SetPriority(unsigned char priority) {
	if(priority > MaxPriority)
		return;

	SCB_SHPR3 = (SCB_SHPR3 & ~SCB_SHPR3_PRI_14_MASK) |
			SCB_SHPR3_PRI_14(priority << PriorityShift);

	/* Enable interrupts from PDB0 and set its interrupt priority */
	NVICEnableIRQ(PendSV_IRQn, priority);
}

void sysTickTimerInit()
{
	/* Systick timer It is a 24-bit, clear-on-write, decrementing, wrap-on-zero counter. */

	/* Address      Name   Type Reset      Description
	 * 
	 * 0xE000E010 SYST_CSR RW 0x0000000xa SysTick Control and Status Register, SYST_CSR on page B3-746
	 * 0xE000E014 SYST_RVR RW UNKNOWN     SysTick Reload Value Register, SYST_RVR on page B3-747
	 * 0xE000E018 SYST_CVR RW UNKNOWN     SysTick Current Value Register, SYST_CVR on page B3-748
	 * 
	 */

	/* After calling mcgInit(), the processor clock is set to 120 MHz and, as a result,
	 * SysTick will be decremented once every 1/120,000,000 seconds, or 
	 * 8.3333 x 10^(-9), or 8.3333 nanoseconds. Because the largest 24-bit 
	 * reset value (stored in SYST_RVR) is 2^24, or 16,777,216, that implies 
	 * that the maximum period for interrupts from the SysTick timer is 
	 * 16,777,216 x 8.3333 nanoseconds, or 139.81013333 milliseconds.
	 * Therefore, it should be clear that this timer supports all reasonable
	 * quantum interrupt periods.
	 * */

	/* For our quantum interrupt purpose 
	 * 
	 * Lets say we want the reasonable quantum interrupt to occur at 50 msec 
	 * 
	 * 50 msec / 8.33 nsec = (6002400 reload value)
	 * 
	 * That means the the process switching overhead should be less than 5% of that
	 * i.e. less than 50*5/100 msec = 2.5 msec
	 * 
	 * which is 300120 instruction cycles, and is reasonable
	 */

	SYST_RVR = SysTick_RVR_RELOAD(6002400);
	SYST_CVR = SysTick_CVR_CURRENT(0);

	/* SysTick_CSR_TICKINT_MASK Starts the interrupt */
	SYST_CSR |= SysTick_CSR_TICKINT_MASK | SysTick_CSR_CLKSOURCE_MASK;

	/* Set Priority*/
	sysTickInit_SetPriority(SYSTICK_INTERRUPT_PRIORITY);
}

void pendSVInit()
{
	/* pendSVInit_SetPriority(14)*/
	pendSVInit_SetPriority(PENDSV_INTERRUPT_PRIORITY);

	/* Start PendSV Interrupt. */
	SCB_ICSR |= SCB_ICSR_PENDSVSET_MASK;  
}

void sysTickTimerStart()
{
	/* Enables the timer */
	SYST_CSR |= SysTick_CSR_ENABLE_MASK ;
}

void sysTickHandler()
{
	/* Set the pendSV bit of the ICSR Register*/
	SCB_ICSR |= SCB_ICSR_PENDSVSET_MASK;
}

void reloadSysTickTimerRVR()
{
	/* Quantum Interrupt of 50 ms */
	SYST_RVR = SysTick_RVR_RELOAD(6002400);
}

void pauseSysTickTimer()
{
	/* The SysTick timer can be paused.  This would allow your OS to give
	 * a process a full quantum even if it were interrupted by interrupt
	 * service routines.  For example, the SysTick timer could be paused when
	 * entering an ISR and resumed when leaving an ISR. Beware: you need to
	 * be sure that you correctly handle nested ISRs.
	 */	
	if(sysTickPauseCount==0)
	{
		/* Pause the systick timer if you are in ISR */
		SYST_CSR &= ~SysTick_CSR_ENABLE_MASK ;
		sysTickPauseCount++;
	}
	else
	{
		sysTickPauseCount++;
	}
}

void resumeSysTickTimer()
{
	if(sysTickPauseCount==1)
	{
		/* Start a timer from where you left off */
		sysTickTimerStart();
		sysTickPauseCount--;
	}
	else
	{
		sysTickPauseCount--;
	}
}

void resetSysTickTimer()
{
	/* The SysTick timer can be reset to zero (by writing any value into 
	 * SYST_CVR).  This would allow your OS to give each new process a full
	 * quantum even if the previous process used only a portion of its
	 * quantum.
	 * 
	 */
	SYST_CVR = 0;
}
