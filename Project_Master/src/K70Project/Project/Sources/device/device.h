/*
 * device.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stddef.h>
#include <stdint.h>
#include "../util/util.h"
#include "derivative.h"
#include "led.h"
#include "pushbutton.h"
#include "uart.h"
#include "deviceEnums.h"
#include "deviceFcnDeclaration.h"
#include "../svc/svc.h"
#include "lcdcConsole.h"
#include "MK70F12.h"
#include "morse.h"
#include "../int/PDB0.h"
#include "../int/intSerialIO.h"

bool isaDefaultStream(char*);
int getListID(char *);
bool isaDevice(char *);
int isaFile(char *);
int getFileListID(char *);
int getDeviceListID(char *);
void* getMinorDeviceStruct(char*,int,int);
int getOpenStreamIDifAlreadyOpen(void*,char*);
void displayAlreadyOpenMessage(char*);
int getStreamID(char* ,char*);
uint32_t getAllfileNodesDataSize(void*);
void create_file(char *);
int delete_file(char *);
bool eofReached(int);

struct deviceEntry
{
	char *name; 
	int majorDeviceID;
	int minorDeviceID;
} allDevices[] = {
		{"/dev/led/orange", 		 LED,		 ORANGE},
		{"/dev/led/yellow", 		 LED, 		 YELLOW},
		{"/dev/led/blue",   		 LED, 		 BLUE},
		{"/dev/led/green",  		 LED, 		 GREEN},
		{"/dev/pushbutton/pb1",  	 PUSHBUTTON, PB1},
		{"/dev/pushbutton/pb2", 	 PUSHBUTTON, PB2},
		{"",                         FILESYSTEM, FILE_REGULAR},     /* FILE_ID = 6 */
		{"/dev/uart/2", 	         UART_COMM,  UART_2},           /* UART2_ID = 7 */
		{"/dev/lcdc/rgb",            LCDC_DEV,   RGB},
		{"/dev/adc/pot",             ADC,        POTENTIOMETER},
		{"/dev/adc/temp",            ADC,        TEMPRATURE_SENSOR},
		{"/dev/touch/ts1",           TOUCH,      TS1},
		{"/dev/touch/ts2",           TOUCH,      TS2},
		{"/dev/touch/ts3",           TOUCH,      TS3},
		{"/dev/touch/ts4",           TOUCH,      TS4},
		{"/dev/morse/led/orange",    MORSE_LED,  ORANGE},
		{"/dev/morse/led/yellow", 	 MORSE_LED,	 YELLOW},
		{"/dev/morse/led/blue",   	 MORSE_LED,	 BLUE},
		{"/dev/morse/led/green",  	 MORSE_LED,	 GREEN},
};

#define TOTAL_NUMBER_OF_DEVICES (sizeof(allDevices)/sizeof(allDevices[1]))

/************************************************************/
/* Base structure for the majorDevice */
struct device
{
	int(*myfgetc)(void *);
	int(*myfputc)(char,void *);
	int(*myfopen)(void *);
	int(*myfclose)(void *);	
} majorDevice[DEVICE_MAX_INDEX] = {
		
		{ /* This is instance of a device_fcn structure for specifying functions for class of major device */
				led_fgetc, /* led_fgetc will interpret the void pointer be a led specific memory*/
				led_fputc,
				led_fopen, /* make sure that clock is set for the LED */
				led_fclose
		}, 
		{
				pushbutton_fgetc,
				pushbutton_fputc, /* Ignores the character because you can not push the button from processor */
				pushbutton_fopen,
				pushbutton_fclose
		},
		{
				file_fgetc,
				file_fputc,
				file_fopen,
				file_fclose
		},
		{
				uart_fgetc,
				uart_fputc,
				uart_fopen,
				uart_fclose
		},
		{
				lcdc_fgetc,
				lcdc_fputc,
				lcdc_fopen,
				lcdc_fclose
		},
		{
				adc_fgetc,
				adc_fputc,
				adc_fopen,
				adc_fclose
		},
		{       ts_fgetc,
				ts_fputc,
				ts_fopen,
				ts_fclose
		},
		{
				morse_led_fgetc,
				morse_led_fputc,
				morse_led_fopen,
				morse_led_fclose
		}
};


/* In order to set and clear the LED bit for port A, 
 * we can use following notion
 * 
 * PTA_BASE_PTR->PCOR
 * PTA_BASE_PTR->PSOR
 * 
 * Hence, they are not included in the LED structure.
 */


/************************************************************/
struct LED
{
	uint32_t  MASK;
};

struct LED led_color[4] = { 
		{ 1<<LED_ORANGE_PORTA_BIT},
		{ 1<<LED_YELLOW_PORTA_BIT},
		{ 1<<LED_BLUE_PORTA_BIT},
		{ 1<<LED_GREEN_PORTA_BIT}		
};

/* This structure is a way to tell major device how to interact with 
 * the particular instance of the device */
/* When User calls led_fputc for doing byte operation on any led
 * that means he wants to either make a led on or off
 */
/************************************************************/
struct PUSHBUTTON
{
	char *port;
	uint32_t  MASK;
};

struct PUSHBUTTON pb_switch[2] = {
		{ "PTD", 1 << PUSHBUTTON_SW1_PORTD_BIT},
		{ "PTE", 1 << PUSHBUTTON_SW2_PORTE_BIT}
};

/************************************************************/
#define ADC_CHANNEL_POTENTIOMETER   	0x14
#define ADC_CHANNEL_TEMPERATURE_SENSOR  0x1A

struct ADCChannel
{
	uint32_t CHANNEL;
}channel[2] = { 
		{ADC_CHANNEL_POTENTIOMETER}, 
		{ADC_CHANNEL_TEMPERATURE_SENSOR}
};


/************************************************************/
#define ELECTRODE_COUNT 4
#define THRESHOLD_OFFSET 0x200

struct electrodeHW {
	int channel;
	uint32_t mask;
	uint16_t threshold;
	uint16_t *counterRegister;
} electrodeHW[ELECTRODE_COUNT] ={
		{5, TSI_PEN_PEN5_MASK, 0, (uint16_t *)&TSI0_CNTR5+1},	/* E1 */
		{8, TSI_PEN_PEN8_MASK, 0, (uint16_t *)&TSI0_CNTR9},	/* E2 */
		{7, TSI_PEN_PEN7_MASK, 0, (uint16_t *)&TSI0_CNTR7+1},	/* E3 */
		{9, TSI_PEN_PEN9_MASK, 0, (uint16_t *)&TSI0_CNTR9+1}};	/* E4 */

/************************************************************/
#define TOTAL_NUMBER_OF_FILES 65535
struct file
{
	bool isFree;
	struct file_node *head; /* Head of the file node - some memory location for the file */
	uint32_t noOfFileNodes; /* Number of file nodes */
	char name[256]; /* Name of the file which may be up to 256 characters */
	/* uint8_t permissions; */ /* May be in future to provide the permissions for file access */
	enum file_type type; /* File or Directory */	
	uint32_t file_size; /* Size of the file which is sum of all the file_node sizes */
};

struct file *allFiles;

/* Global variable which says how many files exists in a system */
int numOfFiles = 0;

/* SINGLe FILE_NODE_DATA_SIZE_LIMIT */
#define SINGLE_FILE_NODE_DATA_SIZE 1018

struct file_node
{
	uint8_t data[SINGLE_FILE_NODE_DATA_SIZE];
	uint16_t filled_size; /* Filled Size for particular node */
	struct file_node *next;	
};

#endif /* DEVICE_H_ */
