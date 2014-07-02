#include "common.h"


BOOL My_DebugBreak()
{
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	DebugBreak ();
	error_Real = GetLastError();
	enableInterception();
	DebugBreak ();
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
