#include "common.h"


BOOL My_RegEnumKeyW()
{
	HKEY hKey=NULL;
	DWORD dwIndex=NULL;
	LPWSTR lpName=NULL;
	DWORD cbName=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegEnumKeyW (hKey,dwIndex,lpName,cbName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegEnumKeyW (hKey,dwIndex,lpName,cbName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
