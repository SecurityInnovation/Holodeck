#include "common.h"


BOOL My_SystemTimeToFileTime()
{
	CONST SYSTEMTIME * lpSystemTime=NULL;
	LPFILETIME lpFileTime=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SystemTimeToFileTime (lpSystemTime,lpFileTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SystemTimeToFileTime (lpSystemTime,lpFileTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
