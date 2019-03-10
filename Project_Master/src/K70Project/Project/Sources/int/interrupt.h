/*
 * interrupt.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Jyot Buch
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void ei(void);
void di(void);

void setInterruptPriorities(void);

#endif /* INTERRUPT_H_ */
