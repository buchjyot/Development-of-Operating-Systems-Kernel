/**
 * svc.h
 * Routines for supervisor calls
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2014, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

#ifndef _SVC_H
#define _SVC_H

#define MaxPriority 15
#define PriorityShift 4

#include "../mem/memory.h"
#include "../device/deviceFcnDeclaration.h"
#include "../int/SVCFlexTimer.h"
#include "../util/reportError.h"
#include "../int/SVCPDBUserTimer.h"
#include "../process/processControlUtils.h"

// Implemented SVC numbers
enum
{
	SVC_MYMALLOC        		= 0,
	SVC_MYFREE          		= 1,
	SVC_MYFREEERRORCODE 		= 2,
	SVC_MYMEMORYMAP     		= 3,
	SVC_FOPEN          			= 4,
	SVC_FCLOSE          		= 5,
	SVC_FGETC           		= 6,
	SVC_FPUTC           		= 7,
	SVC_FGETS           		= 8,
	SVC_FPUTS           		= 9,
	SVC_CREATE          		= 10,
	SVC_DELETE          		= 11,
	SVC_SETTIMEOFDAY    		= 12,
	SVC_GETTIMEOFDAY    		= 13,
	SVC_REPORTERROR     		= 14,
	SVC_SETPDBTIMER     		= 15,
	SVC_GETFREEFILEDISCRIPTOR 	= 16,
	SVC_GETCURRENTPCB  			= 17,
	SVC_GETCURRENTPID			= 18,
	SVC_SPAWN 					= 19,
	SVC_YIELD					= 20,
	SVC_BLOCK					= 21,
	SVC_BLOCKPID				= 22,
	SVC_WAKE					= 23,
	SVC_KILL					= 24,
	SVC_WAIT					= 25,
}svcFunctions;

void svcInit_SetSVCPriority(unsigned char priority);
void svcHandler(void);

void* svc_myMalloc(unsigned int);
void svc_myFree(void*);
int svc_myFreeErrorCode(void*);
void svc_myMemoryMap(void);
int svc_fopen_main(char*, char*);
int svc_fclose_main(int);
int svc_fgetc_main(int);
int svc_fputc_main(int,char);
int svc_fgets_main(int);
int svc_fputs_main(int, char*);
int svc_create(char*);
int svc_delete(char*);
int svc_settimeofday(unsigned long long int);
unsigned long long int svc_gettimeofday(void);
void svc_reportError(int);
void svc_setPDBTimer(int,void*,int);
void* svc_getCurrentPCB(void);
int svc_getFreeFileDiscriptor(void);
pid_t svc_getCurrentPID(void);		
int svc_spawn(void*);
void svc_yield(void);	
void svc_block(void);
int svc_blockPid(pid_t);
int svc_wake(pid_t);	
int svc_kill(pid_t);	
void svc_wait(pid_t);
#endif /* ifndef _SVC_H */
