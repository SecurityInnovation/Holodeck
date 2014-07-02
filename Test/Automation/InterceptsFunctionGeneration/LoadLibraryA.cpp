#include "common.h"


BOOL My_LoadLibraryA()
{
	LPCSTR lpLibFileName=NULL;
	HMODULE returnVal_Real = NULL;
	HMODULE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = LoadLibraryA (lpLibFileName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LoadLibraryA (lpLibFileName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
