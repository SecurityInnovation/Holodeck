#include "common.h"


BOOL My_OpenEventLogA()
{
	LPCSTR lpUNCServerName=NULL;
	LPCSTR lpSourceName=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = OpenEventLogA (lpUNCServerName,lpSourceName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = OpenEventLogA (lpUNCServerName,lpSourceName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
