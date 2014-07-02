#include "common.h"


BOOL My_GlobalAlloc()
{
	UINT uFlags=NULL;
	SIZE_T dwBytes=NULL;
	HGLOBAL returnVal_Real = NULL;
	HGLOBAL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GlobalAlloc (uFlags,dwBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GlobalAlloc (uFlags,dwBytes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	//return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
	return ((GlobalSize(returnVal_Real) == GlobalSize(returnVal_Intercepted)) && (error_Real == error_Intercepted));
}
