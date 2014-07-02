#include "common.h"


BOOL My_GetModuleHandleA()
{
	LPCSTR lpModuleName=NULL;
	HMODULE returnVal_Real = NULL;
	HMODULE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetModuleHandleA (lpModuleName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetModuleHandleA (lpModuleName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
