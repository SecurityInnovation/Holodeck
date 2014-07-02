#include "common.h"


BOOL My_FindFirstFileW()
{
	LPCWSTR lpFileName=NULL;
	LPWIN32_FIND_DATAW lpFindFileData=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = FindFirstFileW (lpFileName,lpFindFileData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FindFirstFileW (lpFileName,lpFindFileData);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
