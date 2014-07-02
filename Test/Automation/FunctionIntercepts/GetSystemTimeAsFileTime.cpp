#include "common.h"


BOOL My_GetSystemTimeAsFileTime()
{
	LPFILETIME lpSystemTimeAsFileTime=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	GetSystemTimeAsFileTime (lpSystemTimeAsFileTime);
	error_Real = GetLastError();
	enableInterception();
	GetSystemTimeAsFileTime (lpSystemTimeAsFileTime);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
