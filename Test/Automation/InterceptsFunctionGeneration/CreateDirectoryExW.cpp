#include "common.h"


BOOL My_CreateDirectoryExW()
{
	LPCWSTR lpTemplateDirectory=NULL;
	LPCWSTR lpNewDirectory=NULL;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateDirectoryExW (lpTemplateDirectory,lpNewDirectory,lpSecurityAttributes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateDirectoryExW (lpTemplateDirectory,lpNewDirectory,lpSecurityAttributes);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
