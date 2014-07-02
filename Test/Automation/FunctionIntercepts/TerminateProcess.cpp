#include "common.h"


BOOL My_TerminateProcess()
{
	HANDLE hProcess=NULL;
	UINT uExitCode=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = TerminateProcess (hProcess,uExitCode);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = TerminateProcess (hProcess,uExitCode);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
