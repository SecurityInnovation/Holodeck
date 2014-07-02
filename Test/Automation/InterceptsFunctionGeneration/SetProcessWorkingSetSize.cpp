#include "common.h"


BOOL My_SetProcessWorkingSetSize()
{
	HANDLE hProcess=NULL;
	SIZE_T dwMinimumWorkingSetSize=NULL;
	SIZE_T dwMaximumWorkingSetSize=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SetProcessWorkingSetSize (hProcess,dwMinimumWorkingSetSize,dwMaximumWorkingSetSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetProcessWorkingSetSize (hProcess,dwMinimumWorkingSetSize,dwMaximumWorkingSetSize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
