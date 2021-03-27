/**
 * util.h
 * routines to perform various utility functions
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Written by James L. Frankel (frankel@seas.harvard.edu)
 *
 * Copyright © 2017, 2015, 2014, 2012 James L. Frankel.  All rights reserved.
 */

#ifndef _UTIL_H
#define _UTIL_H

typedef enum {false = 0, true = !false} bool;

#define STD_DEBOUNCE_DELAY 4000

#include <errno.h>
#include <limits.h>

char currentUser[20];

/* Function prototypes */

unsigned long long strtoull(const char *, char ** , int);

/* Routine to convert an unsigned char value into its corresponding three
 * digit ASCII value.  The returned three ASCII chars are placed in the first
 * three characters pointed to by "string." */
void char2ascii(unsigned char ch, char *string);

/* Routine to convert a nibble into its hexadecimal ASCII character */
char nibble2hex(unsigned char nibble);

/* Routine to convert an unsigned short int value into its corresponding four
 * character hexadecimal value.  The returned four hexadecimal chars are
 * placed in the first four characters pointed to by "string." */
void shortInt2hex(unsigned short int i, char *string);

/* Routine to convert an unsigned int value into its corresponding eight
 * character hexadecimal value.  The returned eight hexadecimal chars are
 * placed in the first eight characters pointed to by "string." */
void longInt2hex(unsigned long int i, char *string);
unsigned long power(unsigned long, unsigned long);
int mystrlen(char *);

/* Function Declaration */
bool strcmp_fcn(char *, char *);
bool strcpy_fcn(char *, char *);

/* Utility to find the exact match for the command specified */
int stringMatch(char**);

bool isAlphabet(char);
bool isDigit(char);
bool isPrivilegedUsrLogin(void);

#endif /* ifndef _UTIL_H */
