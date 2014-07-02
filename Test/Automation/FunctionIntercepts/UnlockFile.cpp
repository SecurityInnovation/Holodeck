#include "common.h"


BOOL My_UnlockFile()
{
	HANDLE hFile=NULL;
	DWORD dwFileOffsetLow=NULL;
	DWORD dwFileOffsetHigh=NULL;
	DWORD nNumberOfBytesToUnlockLow=NULL;
	DWORD nNumberOfBytesToUnlockHigh=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = UnlockFile (hFile,dwFileOffsetLow,dwFileOffsetHigh,nNumberOfBytesToUnlockLow,nNumberOfBytesToUnlockHigh);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = UnlockFile (hFile,dwFileOffsetLow,dwFileOffsetHigh,nNumberOfBytesToUnlockLow,nNumberOfBytesToUnlockHigh);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
