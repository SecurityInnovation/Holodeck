#include "common.h"


BOOL My_GetDiskFreeSpaceExW()
{
	LPCWSTR lpDirectoryName=NULL;
	PULARGE_INTEGER lpFreeBytesAvailableToCaller=NULL;
	PULARGE_INTEGER lpTotalNumberOfBytes=NULL;
	PULARGE_INTEGER lpTotalNumberOfFreeBytes=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetDiskFreeSpaceExW (lpDirectoryName,lpFreeBytesAvailableToCaller,lpTotalNumberOfBytes,lpTotalNumberOfFreeBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetDiskFreeSpaceExW (lpDirectoryName,lpFreeBytesAvailableToCaller,lpTotalNumberOfBytes,lpTotalNumberOfFreeBytes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
