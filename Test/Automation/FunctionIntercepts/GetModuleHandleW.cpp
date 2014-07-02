#include "common.h"


BOOL My_GetModuleHandleW()
{
	LPCWSTR lpModuleName=NULL;
	HMODULE returnVal_Real = NULL;
	HMODULE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetModuleHandleW (lpModuleName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetModuleHandleW (lpModuleName);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
