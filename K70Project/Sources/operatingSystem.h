/*
 * operatingSystem.h
 *
 *  Created on: Apr 22, 2017
 *      Author: welcome
 */

#ifndef OPERATINGSYSTEM_H_
#define OPERATINGSYSTEM_H_

#include "mem/memory.h"
#include "util/reportError.h"
#include "svc/svc.h"
#include "util/util.h"
#include "svc/priv.h"

typedef enum 
{
	SINGLE_PROCESS_MODE = 1,
	MULTI_PROGRAMMING_MODE = 2
}allModes;

/* Function Prototypes */
int createFirstProcess(int);
int validateUser(bool);
void initalizeDevicesAndMemory(void);
void cleanupDevicesAndMemory(void);
int shell(int argc, char *argv[]);

#endif /* OPERATINGSYSTEM_H_ */
