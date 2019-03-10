/*
 * This program is a error handler, which defines the error codes and their behavior.
 * It displays the error by stderr.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "reportError.h"

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
	{ MAX_ERRORCODE_INDEX,              	"MAXCodeIndex: Only for computing the size of Errorcodes, Not to be used in program"}};
    
    void reportError(errorCode err_code)
    {
        int id;
        id = abs(err_code);
        
        if((id < ERROR_CODE_MAX_INDEX) && (id > 0))
            fprintf(stderr,"%s\n",errordesc[id].message);
        else if(id == 0)
            fprintf(stdout,"%s\n",errordesc[id].message); /* display status */
        else
            fprintf(stderr,"Unknown Error Occured: errorno Code : %d\n",errno); /* If some unknown error occurs then display errno */
    }
