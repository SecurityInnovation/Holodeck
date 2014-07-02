#include "common.h"


BOOL My_DebugBreak()
{
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	DebugBreak ();
	error_Real = GetLastError();
	enableInterception();
	DebugBreak ();
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
