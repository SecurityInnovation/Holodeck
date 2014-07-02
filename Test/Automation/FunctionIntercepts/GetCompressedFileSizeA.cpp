#include "common.h"


BOOL My_GetCompressedFileSizeA()
{
	LPCSTR lpFileName=NULL;
	LPDWORD lpFileSizeHigh=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetCompressedFileSizeA (lpFileName,lpFileSizeHigh);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetCompressedFileSizeA (lpFileName,lpFileSizeHigh);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
