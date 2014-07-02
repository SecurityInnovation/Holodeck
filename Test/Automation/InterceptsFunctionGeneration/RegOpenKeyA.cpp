#include "common.h"


BOOL My_RegOpenKeyA()
{
	HKEY hKey=NULL;
	LPCSTR lpSubKey=NULL;
	PHKEY phkResult=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegOpenKeyA (hKey,lpSubKey,phkResult);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegOpenKeyA (hKey,lpSubKey,phkResult);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
