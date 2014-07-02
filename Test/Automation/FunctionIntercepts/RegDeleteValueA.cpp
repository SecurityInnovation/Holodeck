#include "common.h"


BOOL My_RegDeleteValueA()
{
	HKEY hKey=NULL;
	LPCSTR lpValueName=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = RegDeleteValueA (hKey,lpValueName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegDeleteValueA (hKey,lpValueName);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
