#include "common.h"


BOOL My_GetProcessTimes()
{
	HANDLE hProcess=NULL;
	LPFILETIME lpCreationTime=NULL;
	LPFILETIME lpExitTime=NULL;
	LPFILETIME lpKernelTime=NULL;
	LPFILETIME lpUserTime=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetProcessTimes (hProcess,lpCreationTime,lpExitTime,lpKernelTime,lpUserTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetProcessTimes (hProcess,lpCreationTime,lpExitTime,lpKernelTime,lpUserTime);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
