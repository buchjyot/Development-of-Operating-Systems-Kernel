/*
 * deviceEnums.h
 *
 *  Created on: Apr 8, 2017
 *      Author: welcome
 */

#ifndef _DEVICEENUMS_H_
#define _DEVICEENUMS_H_

#define ON 1
#define OFF 0

typedef enum 
{
	LED = 0,
	PUSHBUTTON = 1,
	FILESYSTEM = 2,
	UART_COMM = 3,
	LCDC_DEV = 4,
	ADC = 5,
	TOUCH = 6,
	MORSE_LED = 7,
	DEVICE_MAX_INDEX = 8
}allMajorDeviceList;

typedef enum
{
	UART2_ID = 7,
	FILE_ID = 6,
}deviceEntryID;

typedef enum 
{
	ORANGE = 0,
	YELLOW = 1,
	BLUE = 2,
	GREEN = 3,
}allLEDs;

typedef enum 
{
	PB1 = 0,
	PB2 = 1,
}allPBs;

typedef enum 
{
	UART_2 = 2,
}allUARTs;

typedef enum
{
	RGB = 0,
}allLCDC;

typedef enum
{
	POTENTIOMETER = 0,
	TEMPRATURE_SENSOR = 1,
}allADC;

typedef enum
{
	TS1 = 0,
	TS2 = 1,
	TS3 = 2,
	TS4 = 3,
}allTouchSensors;

enum file_type
{
	FILE_REGULAR = 0,
	EMPTY = 1,
}allFilesType;

/* Following are the indexes which needs to be used in PCB for indexing in order to findout the components*/
typedef enum
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2,
}streamID;

#endif /* _DEVICEENUMS_H_ */
