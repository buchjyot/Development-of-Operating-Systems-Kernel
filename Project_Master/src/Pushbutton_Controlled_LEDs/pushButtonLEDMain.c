/**
 * main.c
 * Pushbuttons project main program
 *
 * Demonstrates using pushbuttons and LEDs
 * 
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 *
 * Copyright © 2017, 2015, 2014, 2012 James L. Frankel.  All rights reserved.
 */

#include <stdio.h>

#include "delay.h"
#include "led.h"
#include "pushbutton.h"
#define DELAY_VALUE 10000

/* Data structure for ledEntries */
struct ledEntry
{
	void (*On)(void);
	void (*Off)(void);
} colors[] = {
		{ledOrangeOn,ledOrangeOff},
		{ledYellowOn,ledYellowOff},
		{ledGreenOn,ledGreenOff},
		{ledBlueOn,ledBlueOff}
};

int main(void) 
{

	/* Initialize all of the LEDs */
	ledInitAll();
	/* Initialize the pushbuttons */
	pushbuttonSW1Init();

	/* 0->1->2->3->0 */

	int id = 0, flag = 0;

	while(1)
	{
		for(id = 0; id <= 3 ; id++)
		{
			while(1)
			{
				colors[id].On();

				while(sw1In())
				{	
					while(sw1In()) /* Wait for switch being depressed that means while switch is on keep lighting the orange LED */
						colors[id].On();

					/* Leaving the previous loop that means either bouncing effect of depressing effect.
					 * Wait for some time for de-bouncing action to get stabilized */
					delay(DELAY_VALUE);

					/* After some delay, confirm that now switch is really depressed */
					if(!sw1In())
					{
						colors[id].Off();	/* If depressed then turn off the led */
						flag = 1; /* Set the flag to 1*/
						break;
					}
					else 
						continue;
					/* If control reaches here that means the while loop broke due to bouncing effect
					 * Hence, continue to the loop to detect the next pushbutton depress */							
				}
				
				if(flag==1)
				{
					flag = 0; /* reset the flag and break the loop so the next LED can start */
					break;
				}
			}
		}

		id = 0; /* Reload the sequence */
	}

	return 0;
}
