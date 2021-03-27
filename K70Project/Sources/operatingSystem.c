/*
 * This program is a Simple Operating System Implementation in C language
 * Course: Principles of Operating Systems
 * Harvard Extension School CSCI E-92
 *
 * This program includes main() function which allocates main memory and runs a process called shell.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include "operatingSystem.h"

/* Entry point for the OS */
int main()
{
	/* Capture the errorCode */
	errorCode rootStatus;

	/* Initialise Devices and Memory */
	initalizeDevicesAndMemory();

	/* Call infinite While loop to run the shell process continuously. */
	rootStatus = createFirstProcess(SINGLE_PROCESS_MODE);

	/* Cleanup Devices and Memory */
	cleanupDevicesAndMemory();	 

	/* Return the rootStatus */
	return(rootStatus);
}
