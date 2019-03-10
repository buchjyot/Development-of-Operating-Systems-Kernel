/*
 * This header file defines the errorcodes for the error handling.
 */

#ifndef _REPORTERROR_H
#define _REPORTERROR_H

typedef enum
{
    SUCCESS = 0,
    INVALID_INPUT = -1,
    FILE_NOT_FOUND = -2,
    MEMORY_ALLOCATION_FAILED = -3,
    FAILED_TO_EXECUTE_GETTIMEOFDAY = -4,
    INVALID_COMMAND = -5,
    MEMORY_FREE_SUCCESS = -6,
    ATTEMPT_TO_FREE_UNALLOCATED_STORAGE = -7,
    PID_MISSMATCH = -8,
    INVALID_ADDRESS_IN_THE_MEMORY = -9,
    OUT_OF_MAIN_MEMORY = -10,
    NULL_POINTER_NO_EFFECT = -11,
    MAX_ERRORCODE_INDEX = -12,
} errorCode;

/* ERROR_CODE_MAX_INDEX = the errorcode with maximum enum absolute value */
#define ERROR_CODE_MAX_INDEX abs(MAX_ERRORCODE_INDEX)

void reportError(errorCode);

#endif /* ifndef _REPORTERROR_H */
