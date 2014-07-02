#include "common.h"


BOOL My_RegEnumKeyExW()
{
	HKEY hKey=NULL;
	DWORD dwIndex=NULL;
	LPWSTR lpName=NULL;
	LPDWORD cbName=NULL;
	LPDWORD lpReserved=NULL;
	LPWSTR lpClass=NULL;
	LPDWORD lpcbClass=NULL;
	PFILETIME lpftLastWriteTime=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegEnumKeyExW (hKey,dwIndex,lpName,cbName,lpReserved,lpClass,lpcbClass,lpftLastWriteTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegEnumKeyExW (hKey,dwIndex,lpName,cbName,lpReserved,lpClass,lpcbClass,lpftLastWriteTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
