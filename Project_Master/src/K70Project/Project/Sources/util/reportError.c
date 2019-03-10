/*
 * This program is a error handler, which defines the error codes and their behavior.
 * It displays the error by stderr.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../device/uart.h"
#include "reportError.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"

struct errordesc {
	int  code;
	char *message;
} errordesc[] = {
		{ SUCCESS,                  			"The command executed with return status 0." },
		{ INVALID_INPUT,           				"Required number of arguments were not specified by the command." },
		{ FILE_NOT_FOUND,           			"The file that you are trying to acess not found."},
		{ MEMORY_ALLOCATION_FAILED, 			"Failed to allocate the memory."},
		{ FAILED_TO_EXECUTE_GETTIMEOFDAY, 		"Could not execute the gettimeofday command successfully."},
		{ INVALID_COMMAND,               		"The specified command does not exist."},
		{ MEMORY_FREE_SUCCESS,                  "The command executed with return status 0." },
		{ ATTEMPT_TO_FREE_UNALLOCATED_STORAGE,  "You are attempting to free the storage that you have not allocated." },
		{ PID_MISSMATCH,                 		"You are attempting to free the storage which is owned by different PID."},
		{ INVALID_ADDRESS_IN_THE_MEMORY,        "You are attempting to free the storage which is in the valid memory range but not a valid block."},
		{ OUT_OF_MAIN_MEMORY,                   "You are attempting to free the storage which is out of the main memory region."},
		{ NULL_POINTER_NO_EFFECT,	            "You have specified NULL address which has no effect."},
		{ UNBLE_TO_CONFIGURE_UART,              "Can not configure UART for some reason."},
		{ DEVICE_NAME_NOT_FOUND,                "The device name that you specified does not exists."},
		{ FILE_NAME_NOT_FOUND,                  "The file name that you specified does not exists."},
		{ NO_ASSIGNMENT,                        "The function call reaches at the end of the function but nothing assigned to a return value."},
		{ MAX_OPEN_STREMS_LIMIT_REACHED,        "You have reached the maximum open stream limit: File Descriptor not available."},
		{ NOTHING_MATCHED,                      "No device, file or stream found with the name specified."},
		{ ONLY_BYTE_LEVEL_OPERATIONS,           "WARNING: fputc command does only byte level operations(only one character), other specified characters will be ignored."},
		{ DEVICE_ALREADY_OPEN,                  "WARNING: The device that you are trying to open is already open."},
		{ FILE_ALREADY_OPEN,                    "WARNING: The file that you are trying to open is already open."},
		{ DEFAULT_STREAMS_ALREADY_OPEN,         "WARNING: You are trying to open stain/stdout/stderr streams which are already open."},
		{ COPY_FAILURE,                         "Copy failure Occurred."},
		{ INVALID_FILE_DISCRIPTOR,              "The file descriptor that you provided is not valid, please provide a valid file descriptor returned by fopen."},
		{ EOF_REACHED,                          "You have reached end of the file."},
		{ INVALID_FOPEN_MODE,                   "The mode that you specified is not a valid file fopen mode."},
		{ FILE_NOT_OPEN_FOR_WRITE,              "In order to write a file, please open the file with r+, w+ or a mode."},
		{ FILE_NOT_OPEN_FOR_READ,               "In order to read a file, please open the file with r, r+ or w+ mode."},
		{ DO_NOT_CLOSE_DEFAULT_STREAMS,         "You are not allowed to close default streams."},
		{ OUTPUT_ONLY_STREAMS,                  "stdout or stderr corresponding to file descriptor 1 or 2 respectively, are OUTPUT_ONLY_STREAM, You can only write characters to it."},
		{ INPUT_ONLY_STREAMS,                 	"stdin corresponding to file descriptor 0 is INPUT_ONLY_STREAM. You can only read characters from it."},
		{ FGETS_ONLY_FOR_FILES,                 "You can not use fgets to read a string from stdin, instead read chacters individually."},
		{ OUPUT_ONLY_DEVICE,                    "The device that you are trying to access is output only device. You can not read from the device."},
		{ INPUT_ONLY_DEVICE,                    "The device that you are trying to access is input only device. you can not write to the device."},
		{ INVALID_CREDENTIALS,					"Invalid credentials. Please try again."},
		{ USERNAME_MISSMATCH,					"The entered username do not match with database, please try again."},
		{ LOGIN_SUCCESSFULL,					"--------------------- \r\nLogin Successful.\r\n---------------------"},
		{ TERMINATE,               			 	"---------------------\r\nShutDown Successful.\r\n---------------------"},
		{ EPOCH_COUNT_IS_NOT_SET,               "You have not set the EPOCH count, please set this value by calling $ date <countValue>."},
		{ FILE_ALREADY_EXISTS_WITH_SAME_NAME,   "The filename that you are trying to create is associated with existing file. Please chose different name."},
		{ INVALID_MODE,                         "Invalid mode: Please provide valid mode from {r,r+,w,w+,a}."},
		{ USER_MUST_SET_THE_EPOCH_CLOCK,        "The system epoch clock is reset to 0 after reboot. Please initialize the epoch using date command."},
		{ DID_YOU_MEANT_TO_PROVIDE_ISO8601_TIME,"Did you meant to provide ISO 8601 time, Please try again with '2017-04-29T14:30' format."},
		{ CANNOT_ALLOCATE_SIZE_0_OR_NEGETIVE,   "You are trying to allocate the size which is either 0 or negetive, Please provide valid positive size to allocate the memory for."},
		{ INVALID_MORSE_INPUT_CHAR,     		"ERROR: Invalid character input! Can not encode the character to morse code sequence for transmitting over led."},
		{ UNPRIVILEGED_USER_ACTION,				"The action you are performing is not allowed for the unprivileged user.\r\nUse administrator account login to do this action."},
		{ UNABLE_TO_SPAWN_A_PROCESS,            "Could not spawn a process."},
		{ MAX_ERRORCODE_INDEX,              	"MAXCodeIndex: Only for computing the size of errorcodes, Not to be used in program."}};

void SVCreportErrorImpl(errorCode err_code)
{
	int id,len=0;
	id = abs(err_code);
	char buffer[200];

	if((id < abs(ERROR_CODE_MAX_INDEX) && (id > 0)))
	{
		len = snprintf(buffer,200,"%s\r\n",errordesc[id].message);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);
	}
	else if(id == 0)
	{     
		len = snprintf(buffer,200,"%s\r\n",errordesc[id].message);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);
	}
	else
	{         
		len = snprintf(buffer,200,"Unknown Error Occurred: errorno Code : %d\r\n",errno);
		if(len>=0) SVCfputs_mainImpl(STDOUT,buffer);
	}
}
