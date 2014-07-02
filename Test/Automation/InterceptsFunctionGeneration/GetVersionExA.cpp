#include "common.h"


BOOL My_GetVersionExA()
{
	LPOSVERSIONINFOA lpVersionInformation=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetVersionExA (lpVersionInformation);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetVersionExA (lpVersionInformation);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
