#include "common.h"


BOOL My_GetLocalTime()
{
	LPSYSTEMTIME lpSystemTime=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	GetLocalTime (lpSystemTime);
	error_Real = GetLastError();
	enableInterception();
	GetLocalTime (lpSystemTime);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
