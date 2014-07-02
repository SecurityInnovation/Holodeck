#include "common.h"


BOOL My_GetProcessHeaps()
{
	DWORD NumberOfHeaps=NULL;
	PHANDLE ProcessHeaps=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetProcessHeaps (NumberOfHeaps,ProcessHeaps);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetProcessHeaps (NumberOfHeaps,ProcessHeaps);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
