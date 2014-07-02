#include "common.h"


BOOL My_LZDone()
{
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	LZDone ();
	error_Real = GetLastError();
	enableInterception();
	LZDone ();
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
