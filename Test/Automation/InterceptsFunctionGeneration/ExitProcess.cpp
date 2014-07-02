#include "common.h"


BOOL My_ExitProcess()
{
	UINT uExitCode=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	ExitProcess (uExitCode);
	error_Real = GetLastError();
	enableInterception();
	ExitProcess (uExitCode);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
