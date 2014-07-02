#include "common.h"


BOOL My_GetStartupInfoW()
{
	LPSTARTUPINFOW lpStartupInfo=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	GetStartupInfoW (lpStartupInfo);
	error_Real = GetLastError();
	enableInterception();
	GetStartupInfoW (lpStartupInfo);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
