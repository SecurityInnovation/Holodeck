#include "common.h"


BOOL My_SetFileApisToOEM()
{
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	SetFileApisToOEM ();
	error_Real = GetLastError();
	enableInterception();
	SetFileApisToOEM ();
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
