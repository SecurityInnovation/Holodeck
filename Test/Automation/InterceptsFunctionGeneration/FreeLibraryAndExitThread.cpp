#include "common.h"


BOOL My_FreeLibraryAndExitThread()
{
	HMODULE hLibModule=NULL;
	DWORD dwExitCode=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	FreeLibraryAndExitThread (hLibModule,dwExitCode);
	error_Real = GetLastError();
	enableInterception();
	FreeLibraryAndExitThread (hLibModule,dwExitCode);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
