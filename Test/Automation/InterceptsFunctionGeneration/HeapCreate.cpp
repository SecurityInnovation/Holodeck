#include "common.h"


BOOL My_HeapCreate()
{
	DWORD flOptions=NULL;
	SIZE_T dwInitialSize=NULL;
	SIZE_T dwMaximumSize=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = HeapCreate (flOptions,dwInitialSize,dwMaximumSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapCreate (flOptions,dwInitialSize,dwMaximumSize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
