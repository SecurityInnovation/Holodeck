#include "common.h"


BOOL My_CancelIo()
{
	HANDLE hFile=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CancelIo (hFile);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CancelIo (hFile);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
