/*
 * cmd_date command : When user inputs "date" command this command is being
 *					 executed.
 *
 * Expected output:
 *
 *	> date
 *	February, 19, 2017, 23:25:35.123456
 *
 * As any other program this program accepts the argc and argv as an arguments. It
 * checks the validity of the input and calculates the time in HH:MM:SS by converting
 * UNIX time output at GMT. UNIX time output is being calculated from 1 January 1970 using
 * gettimeofday system call.
 *
 * After using the converting logic of time and date it displays the formated string as ouput.
 * Sucessfull completion of this program will return SUCESS after the program execution.
 *
 * Copyright 2017
 * Author: Jyot R. Buch
 *
 */

#include <time.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "reportError.h"

int cmd_date(int argc, char *argv[])
{
    struct myTime
    {
        unsigned int year;
        unsigned int month;
        unsigned int day;
        unsigned int hours;
        unsigned int minutes;
        unsigned int seconds;
        unsigned int microseconds;
    } timeStruct;
    
    struct timeval now;
    int rc;
    char *month_name[12] = 	{"January","February","March","April","May","June","July","August","September","October","November","December"};
    long int epoch, sec_of_day;
    double  years_elapsed, rem_year, months_elapsed, rem_months;
    static int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    /* number of argin check */
    if(argc>1)
        return(INVALID_INPUT);
    
    /* Call gettimeofday to get the current time from from Jan 1st 1970*/
    rc = gettimeofday(&now, NULL);
    if(rc!=0)
        return(FAILED_TO_EXECUTE_GETTIMEOFDAY);
    
    epoch = now.tv_sec;
    /* 1 year (365.24 days) = 31556926 seconds*/
    years_elapsed = epoch/31556926.0;
    timeStruct.year = 1970 + years_elapsed;
    rem_year = years_elapsed - (timeStruct.year - 1970);
    months_elapsed = rem_year*12.0;
    timeStruct.month = months_elapsed;
    rem_months = months_elapsed - timeStruct.month;
    timeStruct.month = timeStruct.month + 1;
    
    if(timeStruct.year%4 == 0) /* Leap Year */
    {
        days_in_month[2] = 29;
        timeStruct.day = rem_months*days_in_month[timeStruct.month-1]+1;
    }
    else
        timeStruct.day = rem_months*days_in_month[timeStruct.month-1]+1;
    
    sec_of_day = epoch % 86400; /* 60*60*24 */
    timeStruct.hours = sec_of_day / (60 * 60);
    timeStruct.minutes = (sec_of_day)% (3600) / 60;
    timeStruct.seconds = epoch % 60;
    timeStruct.microseconds = now.tv_usec;
    
    fprintf(stdout,"%s, %02u, %02u, %02u:%02u:%02u.%u \n",month_name[timeStruct.month-1],timeStruct.day,timeStruct.year,timeStruct.hours,timeStruct.minutes,timeStruct.seconds,timeStruct.microseconds);
    
    return(SUCCESS);
}
