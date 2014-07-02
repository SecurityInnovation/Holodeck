#include "common.h"


BOOL My__llseek()
{
	HFILE hFile=NULL;
	LONG lOffset=NULL;
	int iOrigin=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = _llseek (hFile,lOffset,iOrigin);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = _llseek (hFile,lOffset,iOrigin);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
