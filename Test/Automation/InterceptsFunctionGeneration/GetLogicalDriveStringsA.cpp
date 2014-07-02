#include "common.h"


BOOL My_GetLogicalDriveStringsA()
{
	DWORD nBufferLength=NULL;
	LPSTR lpBuffer=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetLogicalDriveStringsA (nBufferLength,lpBuffer);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetLogicalDriveStringsA (nBufferLength,lpBuffer);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
