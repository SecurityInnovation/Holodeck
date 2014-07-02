#include "common.h"


BOOL My_GetSystemTime()
{
	LPSYSTEMTIME lpSystemTime=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	GetSystemTime (lpSystemTime);
	error_Real = GetLastError();
	enableInterception();
	GetSystemTime (lpSystemTime);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
