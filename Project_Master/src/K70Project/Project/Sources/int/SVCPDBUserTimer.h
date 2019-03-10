/**
 * intPDBOneShotTest.h
 * Programmable Delay Block (PDB) one shot interrupt test program
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2014, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 */

#ifndef _INT_PDB_ONE_SHOT_H
#define _INT_PDB_ONE_SHOT_H

void SVCsetPDBTimerImpl(int,void*,int);

/* Data structure for commands */
struct userFcnEntry
{
    void (*functionp)(void);
}user_fcn;

#endif /* ifndef _INT_PDB_ONE_SHOT_TEST_H */
