/*
 * cmd_date command : When user inputs "date" command this command is being
 *					 executed.
 *
 * Expected output:
 *
 *	> date
 *	February, 19, 2017, 23:25:35
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

#include <stdio.h>
#include <stdlib.h>
#include "../svc/svc.h"
#include "../util/util.h"
#include "../util/reportError.h"

/*
 * #define SECONDS_IN_400_YEARS 12622780800
 * #define SECONDS_IN_100_YEARS  3153600000
 */
#define SECONDS_IN_4_YEARS              126227808
#define SECONDS_IN_1_NONLEAP_YEAR        31536000
#define SECONDS_IN_1_LEAP_YEAR   		 31622400
#define SECONDS_IN_A_DAY                 24*60*60
#define SECONDS_IN_ONE_HOUR      	         3600
#define SECONDS_IN_ONE_MINUTE      	           60

#define EPOCH_YEAR 1970
#define EPOCH_MONTH 1
#define EPOCH_DAY 1

struct myTime
{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
}timeStruct;

int isaLeapYear(int);
int containAnyColonsOrDashes(char*);
int isaISO8601Time(char*);
int isaISO8601Time(char*);
void parseISO8601String(char*,struct myTime*);
unsigned long long int convertToEpochTime(struct myTime*);

int cmd_date(int argc, char *argv[])
{

	int k,n;
	char *month_name[12] = 	{"January","February","March","April","May","June","July","August","September","October","November","December"};
	const int daysInMonthNonLeapYear[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	const int daysInMonthLeapYear[] = {31,29,31,30,31,30,31,31,30,31,30,31};
	long long int returnVal,epoch,temp,sec_of_day;
	char buffer[50];

	/* number of argin check */
	if(argc>2)
		return(INVALID_INPUT);

	switch(argc)
	{

	case 1:		
		returnVal = svc_gettimeofday();
		if(returnVal<0)
			return(returnVal);
		else
			epoch = returnVal/1000; /* Following code is as per the seconds */

		/* timeStruct init */
		timeStruct.year = EPOCH_YEAR;
		timeStruct.month = EPOCH_MONTH;
		timeStruct.day   = EPOCH_DAY;
		timeStruct.hours = 0;
		timeStruct.minutes = 0;
		timeStruct.seconds = 0;

		/* Number of seconds from 1 Jan 1970, 00:00:00 */  		
		temp = epoch;

		/* 
		 * NOTE: Since our processor is 32 bit we are not comparing 400 and 100 years seconds 
		 */		

		/*for(i=0;i<temp/SECONDS_IN_400_YEARS;i++){}*/
		/* Substract 400 year seconds if i = 1 and such */
		/*temp = temp - (i*SECONDS_IN_400_YEARS);
		timeStruct.year = timeStruct.year + (i*400);*/

		/*for(j=0;j<temp/SECONDS_IN_100_YEARS;j++){}*/
		/* Substract 100 year seconds if j = 1 and such */
		/*temp = temp - (j*SECONDS_IN_100_YEARS);
		timeStruct.year = timeStruct.year + (j*100);*/

		k=temp/SECONDS_IN_4_YEARS;		
		/* Substract 4 year's seconds if k = 1 and such */
		temp = temp - (k*SECONDS_IN_4_YEARS);
		timeStruct.year = timeStruct.year + (k*4);

		while(temp/SECONDS_IN_1_LEAP_YEAR)
		{
			if(isaLeapYear(timeStruct.year))
			{
				temp = temp - SECONDS_IN_1_LEAP_YEAR;
				timeStruct.year = timeStruct.year + 1;
			}
			else
			{
				temp = temp - SECONDS_IN_1_NONLEAP_YEAR;
				timeStruct.year = timeStruct.year + 1;
			}	
		}

		/* Step through the months */
		if(isaLeapYear(timeStruct.year))
		{	
			while(temp/(daysInMonthLeapYear[timeStruct.month-1]*SECONDS_IN_A_DAY))
			{
				temp = temp - (daysInMonthLeapYear[timeStruct.month-1]*SECONDS_IN_A_DAY);
				timeStruct.month = timeStruct.month + 1;
			}
		}
		else
		{
			while(temp/(daysInMonthNonLeapYear[timeStruct.month-1]*SECONDS_IN_A_DAY))
			{
				temp = temp - (daysInMonthNonLeapYear[timeStruct.month-1]*SECONDS_IN_A_DAY);
				timeStruct.month = timeStruct.month + 1;
			}
		}		

		/* Step Through days in mth month */
		for(n=1;temp/(SECONDS_IN_A_DAY*n);n++){}
		if(n==1)timeStruct.day = timeStruct.day;
		else timeStruct.day = timeStruct.day + n - 1;

		/* After getting date now calculate time */
		sec_of_day = epoch % 86400; /* 60*60*24 */
		timeStruct.hours = sec_of_day / (60 * 60);
		timeStruct.minutes = (sec_of_day)% (3600) / 60;
		timeStruct.seconds = epoch % 60;

		int len = snprintf(buffer,50,"%s, %02u, %02u, %02u:%02u:%02u \r\n",month_name[timeStruct.month-1],timeStruct.day,timeStruct.year,timeStruct.hours,timeStruct.minutes,timeStruct.seconds);
		if(len>=0) svc_fputs_main(STDOUT,buffer);
		break;

	case 2:/* User is trying to set the timeofday clock */
		if(!isPrivilegedUsrLogin()) /* Only the privileged users are allowed to set the time for the OS */
		{
			return(UNPRIVILEGED_USER_ACTION);
		}
		else if(isaISO8601Time(argv[1]))
		{
			/* If the year is <1970 or the value is negative, the relationship is undefined.
			 * If the year is >=1970 and the value is non-negative, the value is related to
			 * a Coordinated Universal Time name according to the C-language expression, where
			 * tm_sec, tm_min, tm_hour, tm_yday, and tm_year are all integer types:*/

			/* 2017-04-29T14:30 */
			struct myTime *tm = (struct myTime*)svc_myMalloc(sizeof(struct myTime));

			parseISO8601String(argv[1],tm);			

			unsigned long long int timeInput = convertToEpochTime(tm);
			svc_settimeofday(timeInput*1000); /* Since we are maintaining as milliseconds */
		}
		else
		{
			/* Make sure that user did not meant to provide ISO 8601 Time format by having any dash and colons i.e. any typo error should be rejected*/
			int retCode = containAnyColonsOrDashes(argv[1]);
			if(retCode != SUCCESS) return(retCode);

			/* Convert char to 64 bit integer */
			char* eptr;
			unsigned long long int timeInput = strtoull(argv[1],&eptr,10);		
			svc_settimeofday(timeInput);
		}
		break;

	default:
		break;
	}
	
	return(SUCCESS);
}

int isaLeapYear(int year)
{
	if(year%4 == 0)
	{
		if( year%100 == 0)
		{
			/* year is divisible by 400, hence the year is a leap year*/
			if ( year%400 == 0)
				return(true);
			else
				return(false);
		}
		else
			return(true);
	}
	else
		return(false);
}

int containAnyColonsOrDashes(char* input)
{
	int i;
	for(i=0;i<mystrlen(input);i++)
	{
		if(input[i]==':'||input[i]=='-')
			return(DID_YOU_MEANT_TO_PROVIDE_ISO8601_TIME);
	}

	return(SUCCESS);
}

int isaISO8601Time(char* input)
{
	if(mystrlen(input) == mystrlen("2017-04-29T14:30"))
	{
		if(input[4]=='-' && input[7]=='-' && input[10]=='T' && input[13]==':')
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void parseISO8601String(char* input,struct myTime* new)
{
	/* Set some default values */
	char *year = "2017";
	char *month = "04";
	char *day = "12";
	char *hour = "08";
	char *min = "23";
	int i,j=0;

	/*2017-04-29T14:30*/

	for(i=0;i<4;i++)
		year[i]=input[j++];

	year[i] = '\0';
	j++;

	for(i=0;j<7;i++)
		month[i] = input[j++];

	month[i] = '\0';
	j++;

	for(i=0;j<10;i++)
		day[i] = input[j++];

	day[i] = '\0';
	j++;

	for(i=0;j<13;i++)
		hour[i] = input[j++];

	hour[i] = '\0';
	j++;

	for(i=0;j<16;i++)
		min[i] = input[j++];

	min[i] = '\0';

	new->year = (int)strtol(year, (char **)NULL, 10);
	new->month = (int)strtol(month, (char **)NULL, 10);
	new->day = (int)strtol(day, (char **)NULL, 10);
	new->hours = (int)strtol(hour, (char **)NULL, 10);
	new->minutes = (int)strtol(min, (char **)NULL, 10);
}


unsigned long long int convertToEpochTime(struct myTime* tStruct)
{
	int i,numOf4Years,remYear;
	unsigned long long int count = 0;
	const int daysInMonthNonLeapYear[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	const int daysInMonthLeapYear[] = {31,29,31,30,31,30,31,31,30,31,30,31};

	numOf4Years = (tStruct->year-EPOCH_YEAR)/4;
	count = count + numOf4Years*SECONDS_IN_4_YEARS;

	remYear = (tStruct->year-EPOCH_YEAR)%4;

	while(remYear)
	{
		if(isaLeapYear(tStruct->year-remYear))
			count = count + SECONDS_IN_1_LEAP_YEAR;
		else
			count = count + SECONDS_IN_1_NONLEAP_YEAR;

		remYear--;
	}


	if(isaLeapYear(tStruct->year))
	{
		for(i=0;i<(tStruct->month)-1;i++)
			count = count + (daysInMonthLeapYear[i]*SECONDS_IN_A_DAY);
	}
	else
	{
		for(i=0;i<(tStruct->month)-1;i++)
			count = count + (daysInMonthNonLeapYear[i]*SECONDS_IN_A_DAY);
	}

	count = count + (tStruct->day - 1)*SECONDS_IN_A_DAY;
	count = count + (tStruct->hours)*SECONDS_IN_ONE_HOUR;
	count = count + (tStruct->minutes)*SECONDS_IN_ONE_MINUTE;

	return count;
}
