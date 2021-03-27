/**
 * main.c
 * Demonstration program to illustrate manipulation of the SP register
 * 
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 *
 * Copyright © 2017, 2015, 2014, 2012 James L. Frankel.  All rights reserved.
 * 
 * Last revised 1:14 PM 20-Apr-2017
 */

#include <derivative.h>
#include <stdio.h>
#include <stdint.h>
#include "../util/util.h"
#include "../process/processControlUtils.h"

bool FIRST_PROCESS_FLAG = 1;

uint32_t scheduler(uint32_t oldSP);

/* This section is applicable to the GCC Toolchain.
 *
 * Note that the entry code for this function generates the following
 * assembly code:
 *   push {r4, r7, lr}
 *   sub  sp, sp, #12
 *   add  r7, sp, #0
 *
 * The above code pushes the values of r4, r7, and lr on the stack.
 * r4 is saved because it is used as a temporary register in the
 * generated code.  r7 will be used in the function as a frame pointer
 * to access local automatic variables allocated on the stack.  lr
 * needs to be saved because this function calls other functions
 * (i.e., it is not a leaf function).  It then decrements the sp by
 * 12, thus reserving three 4-byte words on the stack.  Then, it
 * copies the sp into r7.  This allows the generated code to use
 * offsets off of r7 to address automatic variables on the stack.  12
 * bytes are reserved (in addition to the 12 bytes pushed for r4, r7,
 * and lr): (1) one word for copyOfSP, (2) one additional word
 * (unused), and (3) one word in order to maintain double-word
 * alignment (because this function calls other functions).
 *
 * The code generated from "copyOfSP = 0;" is:
 *   mov.w r3, #0
 *   str   r3, [r7, #4]
 *
 * The first line in the code above loads register r3 with the value 0
 * of the expression.  The second line stores register r3 into a word
 * of memory at the address given by the sum of the value in r7 and
 * the constant 4.  This shows us that the compiler is using an offset
 * of 4 off of the fp as the address for the automatic variable
 * copyOfSP.
 *
 * Note that the exit code for this function generates the following
 * assembly code:
 *   add.w r7, r7, #12
 *   mov   sp, r7
 *   pop   {r4, r7, pc}
 *
 * The above code increments r7 by 12 -- this will result in the value
 * that was in the sp just before the entry code of this function was
 * generated.  The value is r7 is copied into the sp -- this
 * effectively pops off the three 4-byte words that were reserved on
 * the stack on entry and restores the sp to its value just before the
 * entry code was executed.  Then, the pop instruction restores the
 * values of r4 and r7 and loads the pc with the value that was in the
 * lr register.  Loading the pc from lr will jump to the return
 * address.
 */

/* This section is applicable to the Freescale Toolchain.
 *
 * Note that the entry code for this function generates the following
 * assembly code:
 *   push {r3,lr}
 *
 * This push instruction saves the value of register LR so that the
 * return address will not be overwritten when other functions (in this
 * case printf) are called.  The push instruction also saves the value
 * of R3 on the stack.  This is *not* done to allow R3 to be saved and
 * restored, but rather so that a single word at the top-of-stack is
 * reserved for my local variable 'copyOfSP'.  If this function were
 * invoked because of an interrupt, the saving and restoring of R3 is
 * already accomplished through the stack frame.
 *
 * The code generated from "copyOfSP = 0;" is:
 *   mov r0,#0
 *   str r0,[sp,#0]
 *
 * The first line in the code above loads register r0 with the value 0
 * of the expression.  The second line stores register r0 into a word
 * of memory at the address given by the sum of the value in sp and
 * the constant 0.  This shows us that the compiler is using an offset
 * of 0 off of the sp as the address for the automatic variable
 * copyOfSP.
 *
 * Note that the exit code for this function generates the following
 * assembly code:
 *   pop {r3,lr}
 *   bx  lr
 *
 * The above code pops the values of r3 and lr off the stack that were
 * pushed by the entry code.  Then, the function returns by jumping to
 * the return address in lr.
 */

/* The PUSH (Push Multiple Registers) instruction is described in
 * the ARMv7-M Architecture Reference Manual ARM DDI 0403E.b
 * (ID120114), Issue E.b, Section A7.7.99 PUSH on page A7-350 */

/* Note that, "The registers are stored in sequence, the
 * lowest-numbered register to the lowest memory address, through to the
 * highest-numbered register to the highest memory address."  This is
 * important for the GCC Toolchain exit code to ensure that the relative
 * position of the LR and PC registers (from the entry push and the exit pop),
 * respectively, is the same.  This is also important for the Freescale
 * Toolchain entry code "push {r3,lr}" because it means that R3 will be on
 * top-of-stack. */

/* This is context switching code */
void pendSVHandler(void) 
{
  uint32_t copyOfSP;

  copyOfSP = 0;

  /* The following assembly language will push registers r4 through
   * r11 onto the stack */
  __asm("push {r4,r5,r6,r7,r8,r9,r10,r11}");

  /* The following assembly language will push the SVCALLACT and
   * SVCALLPENDED bits onto the stack (See Application Note AN6:
   * Interrupt handlers in general and the quantum interrupt handler
   * in particular) */
  __asm("ldr  r0, [%[shcsr]]"     "\n"
	"mov  r1, %[active]"      "\n"
	"orr  r1, r1, %[pended]"  "\n"
	"and  r0, r0, r1"         "\n"
	"push {r0}"
	:
	: [shcsr] "r" (&SCB_SHCSR),
	  [active] "I" (SCB_SHCSR_SVCALLACT_MASK),
	  [pended] "I" (SCB_SHCSR_SVCALLPENDED_MASK)
	: "r0", "r1", "memory", "sp");

  /* As stated in section B5.1 on page B5-728 of the ARMv7-M
   * Architecture Reference Manual ARM DDI 0403E.b (ID120114), Issue
   * E.b, "To support reading and writing the special-purpose
   * registers under software control, ARMv7-M provides three system
   * instructions, CPS, MRS, and MSR. */

  /* The MRS (Move to Register from Special Register) instruction is
   * described in section B5.2.2 on page B5-733 and the MSR (Move to
   * Special Register from ARM Register) instruction is described in
   * section B5.2.3 on page B5-735 */

  /* One web site where the syntax for using advanced features in the
   * 'asm' in-line assembler pseudo-function call is described is:
   * http://www.ethernut.de/en/documents/arm-inline-asm.html */

  /* The LDR (Load Register) instruction is described in section
   * A7.7.44 on page A7-256 and the STR (Store Register) instruction
   * is described in section A7.7.159 on page A7-428 */

  /* The following assembly language will put the current main SP
   * value into the local, automatic variable 'copyOfSP' */
  __asm("mrs %[mspDest],msp" : [mspDest]"=r"(copyOfSP));

  /* Call the scheduler to find the saved SP of the next process to be
   * executed */
  copyOfSP = scheduler(copyOfSP);

  /* The following assembly language will write the value of the
   * local, automatic variable 'copyOfSP' into the main SP */
  __asm("msr msp,%[mspSource]" : : [mspSource]"r"(copyOfSP) : "sp");

  /* The following assembly language will pop the SVCALLACT and
   * SVCALLPENDED bits off the stack (See Application Note AN6:
   * Interrupt handlers in general and the quantum interrupt handler
   * in particular) */
  __asm("pop {r0}"               "\n"
	"ldr r1, [%[shcsr]]"     "\n"
	"bic r1, r1, %[active]"  "\n"
	"bic r1, r1, %[pended]"  "\n"
	"orr r0, r0, r1"         "\n"
	"str r0, [%[shcsr]]"
	:
	: [shcsr] "r" (&SCB_SHCSR),
	  [active] "I" (SCB_SHCSR_SVCALLACT_MASK),
	  [pended] "I" (SCB_SHCSR_SVCALLPENDED_MASK)
	: "r0", "r1", "sp", "memory");

  /* The POP (Pop Multiple Registers) instruction is described in
   * the ARMv7-M Architecture Reference Manual ARM DDI 0403E.b
   * (ID120114), Issue E.b, Section A7.7.99 PUSH on page A7-348 */

  /* The following assembly language will pop registers r4 through
   * r11 off of the stack */
  __asm("pop {r4,r5,r6,r7,r8,r9,r10,r11}");
}

uint32_t scheduler(uint32_t oldSP) {
	/* Scheduler is a function which decides what should be the currentPCB*/
	if(queryFirstProcessFlag())
		{ 
			/* Throw away the first stack pointer, since it refers to main stack */
			FIRST_PROCESS_FLAG = false; /* Toggle so that next time you do not end up here */
			return(currentPCB->stackData.SP); /* You will only end up here once */
			/* Return currentPCB's SP so that it starts poping off the registers for the process 1 */
		}
	
	/* Store oldSP as the currentPCB's Sp, so when we will comeback for this process, we will resume from here */
	if(currentPCB->process_status == RUNNING)
	{
		currentPCB->stackData.SP = oldSP;
		currentPCB->process_status = READY_TO_RUN; /* Mark as ready to run so that it can be scheduled later */
	}
	
	/* Goto the next PCB*/
	currentPCB = currentPCB->next;
	
	/* Get the next Ready Process */
	uint32_t i;
	unsigned int numOfProcesses = getTotalNumberOfProcesses();
	
	for(i=1;i<numOfProcesses;i++)
	{
		if(!(currentPCB->process_status == READY_TO_RUN))
			currentPCB = currentPCB->next;
	}
	
	/* Stack pointer to start the next process from */
	uint32_t newSP = currentPCB->stackData.SP;
	
  return(newSP);
}

bool queryFirstProcessFlag()
{
	return(FIRST_PROCESS_FLAG);
}
