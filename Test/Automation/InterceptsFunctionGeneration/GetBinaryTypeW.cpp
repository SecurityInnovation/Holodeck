#include "common.h"


BOOL My_GetBinaryTypeW()
{
	LPCWSTR lpApplicationName=NULL;
	LPDWORD lpBinaryType=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetBinaryTypeW (lpApplicationName,lpBinaryType);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetBinaryTypeW (lpApplicationName,lpBinaryType);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
