#include "common.h"


BOOL My_FindNextFileA()
{
	HANDLE hFindFile=NULL;
	LPWIN32_FIND_DATAA lpFindFileData=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = FindNextFileA (hFindFile,lpFindFileData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FindNextFileA (hFindFile,lpFindFileData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
