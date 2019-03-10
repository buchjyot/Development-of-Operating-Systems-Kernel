#include "reportError.h"
#include <stdio.h>

/* function declaration */
int strcmp_fcn(char *, char *);

int cmd_strcmp(int argc, char *argv[])
{
    errorCode status;
    
    if(argc != 3)
        return(INVALID_INPUT);
    
    status = strcmp_fcn(argv[1],argv[2]);
    
    if(status == 1)
        fprintf(stdout,"%s\n", "Strings are equal."); /* display status if equal */
    else
        fprintf(stdout,"%s\n", "Strings are not equal."); /* display status if not equal */
    
    return(SUCCESS);
}

int strcmp_fcn(char *str1, char *str2)
{
    while (*str1 && *str2 && *str1 == *str2) { ++str1; ++str2; }
    if(*str1 - *str2 == 0)
        return(1); /* return 1 if both the strings are equal */
    else
        return(0); /* return 0 if both strings are different */
}
