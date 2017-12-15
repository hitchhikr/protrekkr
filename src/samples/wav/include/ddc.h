/*
   This file is part of the Buzé base Buzz-library. 
   
   Please refer to LICENSE.TXT for details regarding usage.
*/

/*============================================================================

    ddc.h  -  Don Cross,  October 1992.

    Generic ddclib stuff.

============================================================================*/

#ifndef _DDC_H_
#define _DDC_H_

// If you add something to DDCRET, please add the appropriate string
// to the function DDCRET_String() in the file 'source\ddcret.cpp'.

enum DDCRET
{
    DDC_SUCCESS,           // The operation succeded
    DDC_FAILURE,           // The operation failed for unspecified reasons
    DDC_OUT_OF_MEMORY,     // Operation failed due to running out of memory
    DDC_FILE_ERROR,        // Operation encountered file I/O error
    DDC_INVALID_CALL,      // Operation was called with invalid parameters
    DDC_USER_ABORT,        // Operation was aborted by the user
    DDC_INVALID_FILE       // File format does not match
};

const char *DDCRET_String(DDCRET);   // See source\ddcret.cpp

typedef int dBOOLEAN;

typedef unsigned char UINT8;
typedef signed char INT8;

typedef unsigned short int UINT16;
typedef signed short int INT16;

#endif /* __DDC_DDC_H */
