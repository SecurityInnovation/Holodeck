#include "common.h"


BOOL My_RegLoadKeyW()
{
	HKEY hKey=NULL;
	LPCWSTR lpSubKey=NULL;
	LPCWSTR lpFile=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = RegLoadKeyW (hKey,lpSubKey,lpFile);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegLoadKeyW (hKey,lpSubKey,lpFile);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
