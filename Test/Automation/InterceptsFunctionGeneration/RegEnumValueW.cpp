#include "common.h"


BOOL My_RegEnumValueW()
{
	HKEY hKey=NULL;
	DWORD dwIndex=NULL;
	LPWSTR lpValueName=NULL;
	LPDWORD lpcbValueName=NULL;
	LPDWORD lpReserved=NULL;
	LPDWORD lpType=NULL;
	LPBYTE lpData=NULL;
	LPDWORD lpcbData=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegEnumValueW (hKey,dwIndex,lpValueName,lpcbValueName,lpReserved,lpType,lpData,lpcbData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegEnumValueW (hKey,dwIndex,lpValueName,lpcbValueName,lpReserved,lpType,lpData,lpcbData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
