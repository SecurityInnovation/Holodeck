#include "common.h"


BOOL My__lcreat()
{
	LPCSTR lpPathName=NULL;
	int iAttribute=NULL;
	HFILE returnVal_Real = NULL;
	HFILE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = _lcreat (lpPathName,iAttribute);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = _lcreat (lpPathName,iAttribute);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
