#include "common.h"


BOOL My_UnlockFileEx()
{
	HANDLE hFile=NULL;
	DWORD dwReserved=NULL;
	DWORD nNumberOfBytesToUnlockLow=NULL;
	DWORD nNumberOfBytesToUnlockHigh=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = UnlockFileEx (hFile,dwReserved,nNumberOfBytesToUnlockLow,nNumberOfBytesToUnlockHigh,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = UnlockFileEx (hFile,dwReserved,nNumberOfBytesToUnlockLow,nNumberOfBytesToUnlockHigh,lpOverlapped);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
