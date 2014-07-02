#include "common.h"


BOOL My_OpenBackupEventLogA()
{
	LPCSTR lpUNCServerName=NULL;
	LPCSTR lpFileName=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = OpenBackupEventLogA (lpUNCServerName,lpFileName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = OpenBackupEventLogA (lpUNCServerName,lpFileName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
