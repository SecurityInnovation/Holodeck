#include "common.h"


BOOL My_RegQueryValueA()
{
	HKEY hKey=NULL;
	LPCSTR lpSubKey=NULL;
	LPSTR lpValue=NULL;
	PLONG lpcbValue=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegQueryValueA (hKey,lpSubKey,lpValue,lpcbValue);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegQueryValueA (hKey,lpSubKey,lpValue,lpcbValue);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
