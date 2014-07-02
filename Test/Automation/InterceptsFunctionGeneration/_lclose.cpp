#include "common.h"


BOOL My__lclose()
{
	HFILE hFile=NULL;
	HFILE returnVal_Real = NULL;
	HFILE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = _lclose (hFile);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = _lclose (hFile);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
