#include "common.h"


BOOL My_ExitProcess()
{
	UINT uExitCode=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	ExitProcess (uExitCode);
	error_Real = GetLastError();
	enableInterception();
	ExitProcess (uExitCode);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
