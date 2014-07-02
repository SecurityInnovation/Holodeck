#include "common.h"


BOOL My_RegCreateKeyExW()
{
	HKEY hKey=NULL;
	LPCWSTR lpSubKey=NULL;
	DWORD Reserved=NULL;
	LPWSTR lpClass=NULL;
	DWORD dwOptions=NULL;
	REGSAM samDesired=NULL;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL;
	PHKEY phkResult=NULL;
	LPDWORD lpdwDisposition=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegCreateKeyExW (hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,phkResult,lpdwDisposition);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegCreateKeyExW (hKey,lpSubKey,Reserved,lpClass,dwOptions,samDesired,lpSecurityAttributes,phkResult,lpdwDisposition);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
