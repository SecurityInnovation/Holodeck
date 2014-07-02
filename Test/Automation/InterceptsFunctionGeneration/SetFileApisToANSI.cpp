#include "common.h"


BOOL My_SetFileApisToANSI()
{
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	SetFileApisToANSI ();
	error_Real = GetLastError();
	enableInterception();
	SetFileApisToANSI ();
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
