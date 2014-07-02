#include "common.h"


BOOL My_FindFirstFileA()
{
	LPCSTR lpFileName=NULL;
	LPWIN32_FIND_DATAA lpFindFileData=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = FindFirstFileA (lpFileName,lpFindFileData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FindFirstFileA (lpFileName,lpFindFileData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
