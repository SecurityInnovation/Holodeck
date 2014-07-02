#include "common.h"


BOOL My_SetProcessShutdownParameters()
{
	DWORD dwLevel=NULL;
	DWORD dwFlags=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SetProcessShutdownParameters (dwLevel,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetProcessShutdownParameters (dwLevel,dwFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
