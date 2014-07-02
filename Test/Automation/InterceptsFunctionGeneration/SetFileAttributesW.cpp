#include "common.h"


BOOL My_SetFileAttributesW()
{
	LPCWSTR lpFileName=NULL;
	DWORD dwFileAttributes=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SetFileAttributesW (lpFileName,dwFileAttributes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetFileAttributesW (lpFileName,dwFileAttributes);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
