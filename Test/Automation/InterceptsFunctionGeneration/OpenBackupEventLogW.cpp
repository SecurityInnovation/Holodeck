#include "common.h"


BOOL My_OpenBackupEventLogW()
{
	LPCWSTR lpUNCServerName=NULL;
	LPCWSTR lpFileName=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = OpenBackupEventLogW (lpUNCServerName,lpFileName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = OpenBackupEventLogW (lpUNCServerName,lpFileName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
