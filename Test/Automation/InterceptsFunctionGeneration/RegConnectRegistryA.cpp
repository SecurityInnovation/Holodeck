#include "common.h"


BOOL My_RegConnectRegistryA()
{
	LPCSTR lpMachineName=NULL;
	HKEY hKey=NULL;
	PHKEY phkResult=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegConnectRegistryA (lpMachineName,hKey,phkResult);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegConnectRegistryA (lpMachineName,hKey,phkResult);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
