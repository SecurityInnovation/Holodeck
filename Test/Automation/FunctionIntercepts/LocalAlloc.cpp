#include "common.h"


BOOL My_LocalAlloc()
{
	UINT uFlags=NULL;
	SIZE_T uBytes=NULL;
	HLOCAL returnVal_Real = NULL;
	HLOCAL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = LocalAlloc (uFlags,uBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LocalAlloc (uFlags,uBytes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((LocalSize(returnVal_Real) == LocalSize(returnVal_Intercepted)) && (error_Real == error_Intercepted));
}
