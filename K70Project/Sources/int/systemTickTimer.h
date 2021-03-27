/*
 * systemTickTimer.h
 *
 *  Created on: May 2, 2017
 *      Author: Jyot Buch
 */

#ifndef SYSTEMTICKTIMER_H_
#define SYSTEMTICKTIMER_H_

void sysTickInit_SetPriority(unsigned char);
void pendSVInit_SetPriority(unsigned char);
void sysTickTimerInit(void);
void sysTickTimerStart(void);
void pendSVInit(void);

#endif /* SYSTEMTICKTIMER_H_ */
