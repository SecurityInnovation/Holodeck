#include "common.h"


BOOL My_RegOverridePredefKey()
{
	HKEY hKey=NULL;
	HKEY hNewKey=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegOverridePredefKey (hKey,hNewKey);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegOverridePredefKey (hKey,hNewKey);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
