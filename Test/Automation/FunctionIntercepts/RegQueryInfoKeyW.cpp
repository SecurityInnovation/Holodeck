#include "common.h"


BOOL My_RegQueryInfoKeyW()
{
	HKEY hKey=NULL;
	LPWSTR lpClass=NULL;
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
	__try{
	disableInterception();
	returnVal_Real = RegQueryInfoKeyW (hKey,lpClass,lpcbClass,lpReserved,lpcSubKeys,lpcbMaxSubKeyLen,lpcbMaxClassLen,lpcValues,lpcbMaxValueNameLen,lpcbMaxValueLen,lpcbSecurityDescriptor,lpftLastWriteTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegQueryInfoKeyW (hKey,lpClass,lpcbClass,lpReserved,lpcSubKeys,lpcbMaxSubKeyLen,lpcbMaxClassLen,lpcValues,lpcbMaxValueNameLen,lpcbMaxValueLen,lpcbSecurityDescriptor,lpftLastWriteTime);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
