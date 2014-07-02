#include "common.h"


BOOL My_GetSystemInfo()
{
	LPSYSTEM_INFO lpSystemInfo=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	GetSystemInfo (lpSystemInfo);
	error_Real = GetLastError();
	enableInterception();
	GetSystemInfo (lpSystemInfo);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
