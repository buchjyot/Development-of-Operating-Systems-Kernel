/**
 * SVCFlexTimer.h
 * FlexTimer interrupt test program
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2014, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

#ifndef _INT_TEST_H
#define _INT_TEST_H

void intFlexTimerInit(void);

void flexTimer0Action(void);

int SVCsettimeofdayImpl(unsigned long long int);

unsigned long long int SVCgettimeofdayImpl(void);

#endif /* ifndef _INT_TEST_H */
