#include "common.h"


BOOL My_RegSaveKeyExW()
{
	HKEY hKey=NULL;
	LPCWSTR lpFile=NULL;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL;
	DWORD Flags=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegSaveKeyExW (hKey,lpFile,lpSecurityAttributes,Flags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegSaveKeyExW (hKey,lpFile,lpSecurityAttributes,Flags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
