#include "common.h"


BOOL My_LocalReAlloc()
{
	HLOCAL hMem=NULL;
	SIZE_T uBytes=NULL;
	UINT uFlags=NULL;
	HLOCAL returnVal_Real = NULL;
	HLOCAL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = LocalReAlloc (hMem,uBytes,uFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LocalReAlloc (hMem,uBytes,uFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
