#include "common.h"


BOOL My_GetDriveTypeW()
{
	LPCWSTR lpRootPathName=NULL;
	UINT returnVal_Real = NULL;
	UINT returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetDriveTypeW (lpRootPathName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetDriveTypeW (lpRootPathName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
