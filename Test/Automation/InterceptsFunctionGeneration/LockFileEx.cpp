#include "common.h"


BOOL My_LockFileEx()
{
	HANDLE hFile=NULL;
	DWORD dwFlags=NULL;
	DWORD dwReserved=NULL;
	DWORD nNumberOfBytesToLockLow=NULL;
	DWORD nNumberOfBytesToLockHigh=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = LockFileEx (hFile,dwFlags,dwReserved,nNumberOfBytesToLockLow,nNumberOfBytesToLockHigh,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LockFileEx (hFile,dwFlags,dwReserved,nNumberOfBytesToLockLow,nNumberOfBytesToLockHigh,lpOverlapped);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
