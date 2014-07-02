#include "common.h"


BOOL My__lwrite()
{
	HFILE hFile=NULL;
	LPCSTR lpBuffer=NULL;
	UINT uBytes=NULL;
	UINT returnVal_Real = NULL;
	UINT returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = _lwrite (hFile,lpBuffer,uBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = _lwrite (hFile,lpBuffer,uBytes);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
