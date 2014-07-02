#include "common.h"


BOOL My_SetFileShortNameA()
{
	HANDLE hFile=NULL;
	LPCSTR lpShortName=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SetFileShortNameA (hFile,lpShortName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetFileShortNameA (hFile,lpShortName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
