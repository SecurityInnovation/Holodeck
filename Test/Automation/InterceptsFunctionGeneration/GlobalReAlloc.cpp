#include "common.h"


BOOL My_GlobalReAlloc()
{
	HGLOBAL hMem=NULL;
	SIZE_T dwBytes=NULL;
	UINT uFlags=NULL;
	HGLOBAL returnVal_Real = NULL;
	HGLOBAL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GlobalReAlloc (hMem,dwBytes,uFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GlobalReAlloc (hMem,dwBytes,uFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
