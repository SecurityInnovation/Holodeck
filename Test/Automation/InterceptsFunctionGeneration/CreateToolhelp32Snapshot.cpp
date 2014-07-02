#include "common.h"


BOOL My_CreateToolhelp32Snapshot()
{
	DWORD dwFlags=NULL;
	DWORD th32ProcessID=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateToolhelp32Snapshot (dwFlags,th32ProcessID);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateToolhelp32Snapshot (dwFlags,th32ProcessID);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
