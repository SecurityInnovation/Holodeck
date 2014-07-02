#include "common.h"


BOOL My_FindNextFileW()
{
	HANDLE hFindFile=NULL;
	LPWIN32_FIND_DATAW lpFindFileData=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = FindNextFileW (hFindFile,lpFindFileData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FindNextFileW (hFindFile,lpFindFileData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
