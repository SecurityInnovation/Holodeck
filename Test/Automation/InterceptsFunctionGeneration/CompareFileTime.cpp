#include "common.h"


BOOL My_CompareFileTime()
{
	CONST FILETIME * lpFileTime1=NULL;
	CONST FILETIME * lpFileTime2=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CompareFileTime (lpFileTime1,lpFileTime2);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CompareFileTime (lpFileTime1,lpFileTime2);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
