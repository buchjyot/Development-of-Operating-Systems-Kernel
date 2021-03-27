/*
 * interruptEnableDisable.c
 *
 *  Created on: Apr 15, 2017
 *      Author: Jyot Buch
 */

/* Maintain the count for how many times interrupt is being disabled or enabled. */
int intCount = 0;

/* Disable interrupts (PRIMASK is set) */
void di()
{
	if(intCount==0)
	{
		__asm("cpsid i");
		intCount++;
	}
	else
	{
		intCount++;
	}
}

/* Allows interrupts (PRIMASK is cleared) */
void ei()
{
	/* Do not actualy enable the interrupt until the count is 1 i.e. only last interrupt remaining */
	if(intCount==1)
	{
		__asm("cpsie i");
		intCount--;
	}
	else
	{
		intCount--;
	}
}
