/**
 * pushbutton.h
 * routines to manipulate the pushbuttons
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 *
 * Copyright © 2017, 2015, 2014, 2012 James L. Frankel.  All rights reserved.
 */

#ifndef _PUSHBUTTON_H
#define _PUSHBUTTON_H

#define PUSHBUTTON_SW1_PORTD_BIT 0

#ifndef PORT_PCR_MUX_ANALOG
#define PORT_PCR_MUX_ANALOG 0
#endif
#ifndef PORT_PCR_MUX_GPIO
#define PORT_PCR_MUX_GPIO 1
#endif

/* Routine to initialize SW1 pushbutton */
/* Note: This procedure *does* enable the appropriate port clocks */
void pushbuttonSW1Init(void);

/* Routine to configure pushbutton SW1 */
/* Note: This procedure does not enable the port clock */
void pushbuttonSW1Config(void);
/* Routine to configure pushbutton SW2 */
/* Note: This procedure does not enable the port clock */

/* Routine to read state of pushbutton SW1 */
int sw1In(void);

#endif /* ifndef _PUSHBUTTON_H */
