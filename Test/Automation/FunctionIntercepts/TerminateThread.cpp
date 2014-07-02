#include "common.h"


BOOL My_TerminateThread()
{
	HANDLE hThread=NULL;
	DWORD dwExitCode=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = TerminateThread (hThread,dwExitCode);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = TerminateThread (hThread,dwExitCode);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
