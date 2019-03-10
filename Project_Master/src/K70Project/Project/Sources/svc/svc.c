/**
 * svc.c
 * Routines for supervisor calls
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2014, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

/*
 * Size of the supervisor call stack frame (no FP extension):
 *   No alignment => 32 (0x20) bytes
 *   With alignment => 36 (0x24) bytes
 *   
 * Format of the supervisor call stack frame (no FP extension):
 *   SP Offset   Contents
 *   +0			 R0
 *   +4			 R1
 *   +8			 R2
 *   +12		 R3
 *   +16		 R12
 *   +20		 LR (R14)
 *   +24		 Return Address
 *   +28		 xPSR (bit 9 indicates the presence of a reserved alignment
 *   				   word at offset +32)
 *   +32		 Possible Reserved Word for Alignment on 8 Byte Boundary
 *   
 * Size of the supervisor call stack frame (with FP extension):
 *   No alignment => 104 (0x68) bytes
 *   With alignment => 108 (0x6C) bytes
 *   
 * Format of the supervisor call stack frame (with FP extension):
 *   SP Offset   Contents
 *   +0			 R0
 *   +4			 R1
 *   +8			 R2
 *   +12		 R3
 *   +16		 R12
 *   +20		 LR (R14)
 *   +24		 Return Address
 *   +28		 xPSR (bit 9 indicates the presence of a reserved alignment
 *   				   word at offset +104)
 *   +32		 S0
 *   +36		 S1
 *   +40		 S2
 *   +44		 S3
 *   +48		 S4
 *   +52		 S5
 *   +56		 S6
 *   +60		 S7
 *   +64		 S8
 *   +68		 S9
 *   +72		 S10
 *   +76		 S11
 *   +80		 S12
 *   +84		 S13
 *   +88		 S14
 *   +92		 S15
 *   +96		 FPSCR
 *   +100		 Reserved Word for 8 Byte Boundary of Extended Frame
 *   +104		 Possible Reserved Word for Alignment on 8 Byte Boundary
 */

#include <derivative.h>
#include <stdio.h>
#include "svc.h"

#define XPSR_FRAME_ALIGNED_BIT 9
#define XPSR_FRAME_ALIGNED_MASK (1<<XPSR_FRAME_ALIGNED_BIT)

struct frame {
	union
	{
		struct
		{
			union {
				int r0;
				int arg0;
				unsigned int uintarg0;
				int returnVal;
				pid_t pidarg0;
				pid_t pidReturnargVal;
				void* voidStarReturnVal;
				void* voidStararg0;
				char* charStararg0;
			};
			union {
				int r1;
				int arg1;
				char chararg1;
				void* voidStararg1;
				char* charStararg1;
			};
		};

		unsigned long long int ulonglongarg0;
		unsigned long long int ulonglongreturnVal;
	};

	union {
		int r2;
		int arg2;
		char** charStarStararg2;
	};
	union {
		int r3;
		int arg3;
		uint32_t uint32targ3;
	};
	int r12;
	union {
		int r14;
		int lr;		
	};
	int returnAddr;
	int xPSR;
};

/**************************************************************************************/
/* Issue the SVC (Supervisor Call) instruction (See A7.7.175 on page A7-503 of the
 * ARM師7-M Architecture Reference Manual, ARM DDI 0403Derrata 2010_Q3 (ID100710)) */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void* __attribute__((naked)) __attribute__((noinline)) svc_myMalloc(unsigned int uintarg0){
	__asm("svc %0" : : "I" (SVC_MYMALLOC));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void* __attribute__((never_inline)) svc_myMalloc(unsigned int uintarg0) {
	__asm("svc %0" : : "I" (SVC_MYMALLOC));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_myFree(void* voidStararg0){
	__asm("svc %0" : : "I" (SVC_MYFREE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_myFree(void* voidStararg0) {
	__asm("svc %0" : : "I" (SVC_MYFREE));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_myFreeErrorCode(void* voidStararg0){
	__asm("svc %0" : : "I" (SVC_MYFREEERRORCODE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_myFreeErrorCode(void* voidStararg0) {
	__asm("svc %0" : : "I" (SVC_MYFREEERRORCODE));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_myMemoryMap(void){
	__asm("svc %0" : : "I" (SVC_MYMEMORYMAP));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_myMemoryMap(void) {
	__asm("svc %0" : : "I" (SVC_MYMEMORYMAP));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fopen_main(char* voidStararg0, char* charStararg1){
	__asm("svc %0" : : "I" (SVC_FOPEN));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fopen_main(char* voidStararg0, char* charStararg1) {
	__asm("svc %0" : : "I" (SVC_FOPEN));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fclose_main(int arg0){
	__asm("svc %0" : : "I" (SVC_FCLOSE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fclose_main(int arg0) {
	__asm("svc %0" : : "I" (SVC_FCLOSE));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fgetc_main(int arg0){
	__asm("svc %0" : : "I" (SVC_FGETC));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fgetc_main(int arg0) {
	__asm("svc %0" : : "I" (SVC_FGETC));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fputc_main(int arg0, char chararg1){
	__asm("svc %0" : : "I" (SVC_FPUTC));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fputc_main(int arg0, char chararg1) {
	__asm("svc %0" : : "I" (SVC_FPUTC));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fgets_main(int arg0){
	__asm("svc %0" : : "I" (SVC_FGETS));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fgets_main(int arg0) {
	__asm("svc %0" : : "I" (SVC_FGETS));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_fputs_main(int arg0, char* charStararg1){
	__asm("svc %0" : : "I" (SVC_FPUTS));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_fputs_main(int arg0, char* charStararg1){
	__asm("svc %0" : : "I" (SVC_FPUTS));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_create(char* charStararg0){
	__asm("svc %0" : : "I" (SVC_CREATE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_create(char* charStararg0){
	__asm("svc %0" : : "I" (SVC_CREATE));
}
#endif

/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_delete(char* charStararg0){
	__asm("svc %0" : : "I" (SVC_DELETE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_delete(char* charStararg0){
	__asm("svc %0" : : "I" (SVC_DELETE));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_settimeofday(unsigned long long int longlongarg0){
	__asm("svc %0" : : "I" (SVC_SETTIMEOFDAY));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_myFree(long long int longlongarg0) {
	__asm("svc %0" : : "I" (SVC_SETTIMEOFDAY));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
unsigned long long int __attribute__((naked)) __attribute__((noinline)) svc_gettimeofday(void){
	__asm("svc %0" : : "I" (SVC_GETTIMEOFDAY));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
long long int __attribute__((never_inline)) svc_myFree(void) {
	__asm("svc %0" : : "I" (SVC_GETTIMEOFDAY));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_reportError(int arg0){
	__asm("svc %0" : : "I" (SVC_REPORTERROR));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_reportError(int arg0) {
	__asm("svc %0" : : "I" (SVC_REPORTERROR));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_setPDBTimer(int arg0,void* voidStararg1, int arg2){
	__asm("svc %0" : : "I" (SVC_SETPDBTIMER));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_setPDBTimer(int arg0,void* voidStararg1,int arg2) {
	__asm("svc %0" : : "I" (SVC_SETPDBTIMER));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_getFreeFileDiscriptor(void){
	__asm("svc %0" : : "I" (SVC_GETFREEFILEDISCRIPTOR));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_getFreeFileDiscriptor(void) {
	__asm("svc %0" : : "I" (SVC_GETFREEFILEDISCRIPTOR));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void* __attribute__((naked)) __attribute__((noinline)) svc_getCurrentPCB(void){
	__asm("svc %0" : : "I" (SVC_GETCURRENTPCB));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void* __attribute__((never_inline)) svc_getCurrentPCB(void) {
	__asm("svc %0" : : "I" (SVC_GETCURRENTPCB));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
pid_t __attribute__((naked)) __attribute__((noinline)) svc_getCurrentPID(void){
	__asm("svc %0" : : "I" (SVC_GETCURRENTPID));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
pid_t __attribute__((never_inline)) svc_getCurrentPID(void) {
	__asm("svc %0" : : "I" (SVC_GETCURRENTPID));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_spawn(void* voidStararg0){
	__asm("svc %0" : : "I" (SVC_SPAWN));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_spawn(void* voidStararg0){
	__asm("svc %0" : : "I" (SVC_SPAWN));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_yield(void){
	__asm("svc %0" : : "I" (SVC_YIELD));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_spawn(void) {
	__asm("svc %0" : : "I" (SVC_YIELD));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_block(void){
	__asm("svc %0" : : "I" (SVC_BLOCK));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_block(void) {
	__asm("svc %0" : : "I" (SVC_BLOCK));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_blockPid(pid_t pidarg0){
	__asm("svc %0" : : "I" (SVC_BLOCKPID));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_blockPid(pid_t pidarg0) {
	__asm("svc %0" : : "I" (SVC_BLOCKPID));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_wake(pid_t pidarg0){
	__asm("svc %0" : : "I" (SVC_WAKE));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_wake(pid_t pidarg0) {
	__asm("svc %0" : : "I" (SVC_WAKE));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int __attribute__((naked)) __attribute__((noinline)) svc_kill(pid_t pidarg0){
	__asm("svc %0" : : "I" (SVC_KILL));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
int __attribute__((never_inline)) svc_kill(pid_t pidarg0) {
	__asm("svc %0" : : "I" (SVC_KILL));
}
#endif
/**************************************************************************************/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
void __attribute__((naked)) __attribute__((noinline)) svc_wait(pid_t pidarg0){
	__asm("svc %0" : : "I" (SVC_WAIT));
	__asm("bx lr");
}
#pragma GCC diagnostic pop
#else
void __attribute__((never_inline)) svc_wait(pid_t pidarg0) {
	__asm("svc %0" : : "I" (SVC_WAIT));
}
#endif

/* This function sets the priority at which the SVCall handler runs (See
 * B3.2.11, System Handler Priority Register 2, SHPR2 on page B3-723 of
 * the ARM師7-M Architecture Reference Manual, ARM DDI 0403Derrata
 * 2010_Q3 (ID100710)).
 * 
 * If priority parameter is invalid, this function performs no action.
 * 
 * The ARMv7-M Architecture Reference Manual in section "B1.5.4 Exception
 * priorities and preemption" on page B1-635 states, "The number of
 * supported priority values is an IMPLEMENTATION DEFINED power of two in
 * the range 8 to 256, and the minimum supported priority value is always 0.
 * All priority value fields are 8-bits, and if an implementation supports
 * fewer than 256 priority levels then low-order bits of these fields are RAZ."
 * 
 * In the K70 Sub-Family Reference Manual in section "3.2.2.1 Interrupt
 * priority levels" on page 85, it states, "This device supports 16 priority
 * levels for interrupts.  Therefore, in the NVIC each source in the IPR
 * registers contains 4 bits."  The diagram that follows goes on to confirm
 * that only the high-order 4 bits of each 8 bit field is used.  It doesn't
 * explicitly mention the System Handler (like the SVC handler) priorities,
 * but they should be handled consistently with the interrupt priorities.
 * 
 * It is important to set the SVCall priority appropriately to allow
 * or disallow interrupts while the SVCall handler is running. */

void svcInit_SetSVCPriority(unsigned char priority) {
	if(priority > MaxPriority)
		return;

	SCB_SHPR2 = (SCB_SHPR2 & ~SCB_SHPR2_PRI_11_MASK) |
			SCB_SHPR2_PRI_11(priority << PriorityShift);
}

void svcHandlerInC(struct frame *framePtr);

/* Exception return behavior is detailed in B1.5.8 on page B1-652 of the
 * ARM師7-M Architecture Reference Manual, ARM DDI 0403Derrata 2010_Q3
 * (ID100710) */

/* When an SVC instruction is executed, the following steps take place:
 * (1) A stack frame is pushed on the current stack (either the main or
 *     the process stack, depending on how the system is configured)
 *     containing the current values of R0-R3, R12, LR, the return
 *     address for the SVC instruction, xPSR, and, if appropriate, the
 *     floating point registers,
 * (2) An appropriate value is loaded into the LR register indicating
 *     that a stack frame is present on the stack (this will cause a
 *     special return sequence later when this value is loaded into
 *     the PC),
 * (3) Values of R0-R3 and R12 are no longer valid,
 * (4) The PC is set to the address in the interrupt vector table for
 * 	   the interrupt service routine for the SVC instruction,
 * (5) The processor switches to Handler mode (code execution in
 *     Handler mode is always privileged),
 * (6) The xPSR is set to indicate appropriate SVC state,
 * (7) The processor switches to the main stack by now using the main
 * 	   stack pointer.
 *     
 * These steps are discussed in detail in the pseudo-code given for
 * processor action ExceptionEntry() on page B1-643 of the ARM師7-M
 * Architecture Reference Manual, ARM DDI 0403Derrata 2010_Q3
 * (ID100710).  ExceptionEntry() invokes PushStack() and
 * ExceptionTaken() on page B1-643. */

/* The following assembler function makes R0 point to the top-of-stack
 * for the stack that was current (either the main or the process stack)
 * before the SVC interrupt service routine began running.  This is
 * where the stack frame was stored by the SVC instruction.  Then, this
 * function calls the svcHandlerInC function.  Note that when a C
 * function is called, R0 contains the first parameter.  Therefore, the
 * first parameter passed to svcHandlerInC is a pointer to the
 * top-of-stack of the stack containing the stack frame. */

#ifdef __GNUC__
void __attribute__((naked)) svcHandler(void) {
	__asm("\n\
            tst		lr, #4\n\
			ite		eq\n\
			mrseq	r0, msp\n\
			mrsne	r0, psp\n\
			push	{lr}\n\
			bl		svcHandlerInC\n\
			pop		{pc}\n\
			");
}
#else
__asm void svcHandler(void) {
	tst		lr, #4
	mrseq	r0, msp
	mrsne	r0, psp
	push	lr
	bl		svcHandlerInC
	pop		pc
}
#endif

void svcHandlerInC(struct frame *framePtr) {

	switch(((unsigned char *)framePtr->returnAddr)[-2])
	{

	case SVC_MYMALLOC:
		framePtr->voidStarReturnVal = SVCmyMallocImpl(framePtr->uintarg0);
		break;

	case SVC_MYFREE:
		SVCmyFreeImpl(framePtr->voidStararg0);
		break;

	case SVC_MYFREEERRORCODE:
		framePtr->returnVal = SVCmyFreeErrorCodeImpl(framePtr->voidStararg0);
		break;

	case SVC_MYMEMORYMAP:
		SVCmyMemoryMapImpl();
		break;

	case SVC_FOPEN:
		framePtr->returnVal = SVCfopen_mainImpl(framePtr->charStararg0,framePtr->charStararg1);
		break;

	case SVC_FCLOSE:
		framePtr->returnVal = SVCfclose_mainImpl(framePtr->arg0);
		break;

	case SVC_FGETC:
		framePtr->returnVal = SVCfgetc_mainImpl(framePtr->arg0);
		break;

	case SVC_FPUTC:
		framePtr->returnVal = SVCfputc_mainImpl(framePtr->arg0,framePtr->chararg1);
		break;

	case SVC_FGETS:
		framePtr->returnVal = SVCfgets_mainImpl(framePtr->arg0);
		break;

	case SVC_FPUTS:
		framePtr->returnVal = SVCfputs_mainImpl(framePtr->arg0,framePtr->charStararg1);
		break;

	case SVC_CREATE:
		framePtr->returnVal = SVCcreateImpl(framePtr->charStararg0);
		break;

	case SVC_DELETE:
		framePtr->returnVal = SVCdeleteImpl(framePtr->charStararg0);
		break;

	case SVC_SETTIMEOFDAY:
		framePtr->returnVal = SVCsettimeofdayImpl(framePtr->ulonglongarg0);
		break;

	case SVC_GETTIMEOFDAY:
		framePtr->ulonglongreturnVal = SVCgettimeofdayImpl();
		break;

	case SVC_REPORTERROR:
		SVCreportErrorImpl(framePtr->arg0);
		break;

	case SVC_SETPDBTIMER:
		SVCsetPDBTimerImpl(framePtr->arg0,framePtr->voidStararg1,framePtr->arg2);
		break;

	case SVC_GETFREEFILEDISCRIPTOR:
		framePtr->returnVal = SVCgetFreeFileDiscriptorImpl();
		break;

	case SVC_GETCURRENTPCB:
		framePtr->voidStarReturnVal = SVCgetCurrentPCBImpl();
		break;

	case SVC_GETCURRENTPID:
		framePtr->pidReturnargVal = SVCgetCurrentPIDImpl();
		break;

	case SVC_SPAWN:
		framePtr->returnVal = SVCspawnImpl(framePtr->voidStararg0);
		break;

	case SVC_YIELD:
		SVCyieldImpl();
		break;

	case SVC_BLOCK:
		SVCblockImpl();
		break;

	case SVC_BLOCKPID:
		framePtr->returnVal = SVCblockPidImpl(framePtr->pidarg0);
		break;

	case SVC_WAKE:
		framePtr->returnVal = SVCwakeImpl(framePtr->pidarg0);
		break;

	case SVC_KILL:
		framePtr->returnVal = SVCkillImpl(framePtr->pidarg0);
		break;

	case SVC_WAIT:
		SVCwaitImpl(framePtr->pidarg0);
		break;

	default:
		SVCfputs_mainImpl(STDOUT,"Unknown SVC has been called\r\n");
	}

	/* Exiting svcHandlerInC */
}
