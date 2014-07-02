#include "common.h"


BOOL My_GetSystemInfo()
{
	LPSYSTEM_INFO lpSystemInfo=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	GetSystemInfo (lpSystemInfo);
	error_Real = GetLastError();
	enableInterception();
	GetSystemInfo (lpSystemInfo);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
