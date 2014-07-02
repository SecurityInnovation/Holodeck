#include "common.h"


BOOL My_RegQueryInfoKeyA()
{
	HKEY hKey=NULL;
	LPSTR lpClass=NULL;
	LPDWORD lpcbClass=NULL;
	LPDWORD lpReserved=NULL;
	LPDWORD lpcSubKeys=NULL;
	LPDWORD lpcbMaxSubKeyLen=NULL;
	LPDWORD lpcbMaxClassLen=NULL;
	LPDWORD lpcValues=NULL;
	LPDWORD lpcbMaxValueNameLen=NULL;
	LPDWORD lpcbMaxValueLen=NULL;
	LPDWORD lpcbSecurityDescriptor=NULL;
	PFILETIME lpftLastWriteTime=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegQueryInfoKeyA (hKey,lpClass,lpcbClass,lpReserved,lpcSubKeys,lpcbMaxSubKeyLen,lpcbMaxClassLen,lpcValues,lpcbMaxValueNameLen,lpcbMaxValueLen,lpcbSecurityDescriptor,lpftLastWriteTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegQueryInfoKeyA (hKey,lpClass,lpcbClass,lpReserved,lpcSubKeys,lpcbMaxSubKeyLen,lpcbMaxClassLen,lpcValues,lpcbMaxValueNameLen,lpcbMaxValueLen,lpcbSecurityDescriptor,lpftLastWriteTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
