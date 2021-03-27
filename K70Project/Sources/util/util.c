/**
 * util.c
 * routines to perform various utility functions
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 * Modified by Jyot Buch
 * Ref: Professor Frankel's course website
 */

#include "reportError.h"
#include "util.h"

#define NULL 0

/* Routine to convert an unsigned char value into its corresponding three
 * digit ASCII value.  The returned three ASCII chars are placed in the first
 * three characters pointed to by "string." */
void char2ascii(unsigned char ch, char *string) {
	string[0] = (char) (ch/100 + '0');
	ch -= ch/100 * 100;
	string[1] = (char) (ch/10 + '0');
	ch -= ch/10 * 10;
	string[2] = (char) (ch + '0');
}

/* Routine to convert a nibble into its hexadecimal ASCII character */
char nibble2hex(unsigned char nibble) {
	if(nibble <= 9) {
		return (char) (nibble + '0');
	} else if(nibble <= 15) {
		return (char) (nibble - 10 + 'A');
	} else {
		return '?';
	}
}

/* Routine to convert an unsigned short int value into its corresponding four
 * character hexadecimal value.  The returned four hexadecimal chars are
 * placed in the first four characters pointed to by "string." */
void shortInt2hex(unsigned short int i, char *string) {
	string[0] = nibble2hex((unsigned char) (i/0x1000));
	i -= i/0x1000 * 0x1000;
	string[1] = nibble2hex((unsigned char) (i/0x100));
	i -= i/0x100 * 0x100;
	string[2] = nibble2hex((unsigned char) (i/0x10));
	i -= i/0x10 * 0x10;
	string[3] = nibble2hex((unsigned char) i);
}

/* Routine to convert an unsigned int value into its corresponding eight
 * character hexadecimal value.  The returned eight hexadecimal chars are
 * placed in the first eight characters pointed to by "string." */
void longInt2hex(unsigned long int i, char *string) {
	string[0] = nibble2hex((unsigned char) (i/0x10000000));
	i -= i/0x10000000 * 0x10000000;
	string[1] = nibble2hex((unsigned char) (i/0x1000000));
	i -= i/0x1000000 * 0x1000000;
	string[2] = nibble2hex((unsigned char) (i/0x100000));
	i -= i/0x100000 * 0x100000;
	string[3] = nibble2hex((unsigned char) (i/0x10000));
	i -= i/0x10000 * 0x10000;
	string[4] = nibble2hex((unsigned char) (i/0x1000));
	i -= i/0x1000 * 0x1000;
	string[5] = nibble2hex((unsigned char) (i/0x100));
	i -= i/0x100 * 0x100;
	string[6] = nibble2hex((unsigned char) (i/0x10));
	i -= i/0x10 * 0x10;
	string[7] = nibble2hex((unsigned char) i);
}

unsigned long power(unsigned long base, unsigned long exp)
{
	unsigned long ret=1;
	int i;
	for(i=0;i<exp;i++){
		ret*=base;
	}
	return ret;
}


/* Function name: mystrlen
 Input parameter: 
      str: a string (character array)
 Returns:
      the number of characters in str,
         not counting the NULL character
 */
int mystrlen(char *str)
{
	int len = 0;
	int i;

	for (i=0; str[i] != '\0'; i++) 
	{
		len++;
	}
	return(len);
}


bool strcmp_fcn(char *str1, char *str2)
{
	int length = 0;
	int strlen1 = mystrlen(str1);
	int strlen2 = mystrlen(str2);

	while (*str1 && *str2 && *str1 == *str2) { ++str1; ++str2; ++length; };

	if ((*str1 - *str2 == 0) && length==strlen1 && length==strlen2)
		return(true); /* return 1 if both the strings are equal */
	else
		return(false); /* return 0 if both strings are different */
}

bool strcpy_fcn(char * dest_ptr, char * src_ptr)
{	
	int i = 0;

	if(src_ptr != '\0')
	{
		/* Start copy src to dest */
		while(src_ptr[i])
		{
			dest_ptr[i] = src_ptr[i];
			i++;
		}
		/* put NULL termination */
		dest_ptr[i] = '\0';
		return(true);
	}

	return(false);
}/* End: strcpy() */

/*
 * Convert a string to an unsigned long long integer.
 *
 * Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
unsigned long long strtoull(const char * __restrict nptr, char ** __restrict endptr, int base)
{
	const char *s;
	unsigned long long acc;
	char c;
	unsigned long long cutoff;
	int neg, any, cutlim;

	/*
	 * See strtoq for comments as to the logic used.
	 */
	s = nptr;
	do {
		c = *s++;
	} while ((unsigned char)c == ' ');
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) &&
			c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	acc = any = 0;
	if (base < 2 || base > 36)
		goto noconv;

	cutoff = ULLONG_MAX / base;
	cutlim = ULLONG_MAX % base;
	for ( ; ; c = *s++) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULLONG_MAX;
		errno = ERANGE;
	} else if (!any) {
		noconv:
		errno = EINVAL;
	} else if (neg)
		acc = -acc;
	if (endptr != NULL)
		*endptr = (char *)(any ? s - 1 : nptr);
	return (acc);
}

bool isAlphabet(char inputChar)
{
	/* Convert to ASCII */
	int ch = (int)inputChar;

	/* ASCII values for capital letters from 65 to 90, ASCII values for small letters from 97 to 122 */
	if((ch>=65 && ch<=90) || (ch>=97 && ch<=122))
		return(true);
	else
		return(false);
}

bool isDigit(char inputChar)
{
	/* Convert to ASCII */
	int dg = (int)inputChar;

	/* ASCII values from 48 to 57 */
	if(dg>=48 && dg<=57)
		return(true);
	else
		return(false);
}

bool isPrivilegedUsrLogin()
{
	return(strcmp_fcn(currentUser,"admin"));
}
