/*
 * cleanupDevicesAndMemory.c
 *
 *  Created on: Apr 22, 2017
 *      Author: Jyot Buch
 */

#include "../mem/memory.h"
#include "../int/flexTimer.h"

void cleanupDevicesAndMemory()
{
	/* Free Main memroy */
	freeMainMemory();
	
	/* Stop the clock */
	flexTimer0Stop();

}

int freeMainMemory()
{
	free(head);
	return(SUCCESS);
}
