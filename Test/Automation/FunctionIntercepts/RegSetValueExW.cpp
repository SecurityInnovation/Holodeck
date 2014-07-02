#include "common.h"


BOOL My_RegSetValueExW()
{
	HKEY hKey=NULL;
	LPCWSTR lpValueName=NULL;
	DWORD Reserved=NULL;
	DWORD dwType=NULL;
	CONST BYTE * lpData=NULL;
	DWORD cbData=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = RegSetValueExW (hKey,lpValueName,Reserved,dwType,lpData,cbData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegSetValueExW (hKey,lpValueName,Reserved,dwType,lpData,cbData);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
