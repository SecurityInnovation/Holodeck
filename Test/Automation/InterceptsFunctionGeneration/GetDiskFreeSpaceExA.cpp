#include "common.h"


BOOL My_GetDiskFreeSpaceExA()
{
	LPCSTR lpDirectoryName=NULL;
	PULARGE_INTEGER lpFreeBytesAvailableToCaller=NULL;
	PULARGE_INTEGER lpTotalNumberOfBytes=NULL;
	PULARGE_INTEGER lpTotalNumberOfFreeBytes=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetDiskFreeSpaceExA (lpDirectoryName,lpFreeBytesAvailableToCaller,lpTotalNumberOfBytes,lpTotalNumberOfFreeBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetDiskFreeSpaceExA (lpDirectoryName,lpFreeBytesAvailableToCaller,lpTotalNumberOfBytes,lpTotalNumberOfFreeBytes);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
