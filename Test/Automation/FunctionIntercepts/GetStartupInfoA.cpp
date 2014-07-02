#include "common.h"


BOOL My_GetStartupInfoA()
{
	LPSTARTUPINFOA lpStartupInfo=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	GetStartupInfoA (lpStartupInfo);
	error_Real = GetLastError();
	enableInterception();
	GetStartupInfoA (lpStartupInfo);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
