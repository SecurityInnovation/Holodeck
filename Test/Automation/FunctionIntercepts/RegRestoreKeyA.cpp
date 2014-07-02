#include "common.h"


BOOL My_RegRestoreKeyA()
{
	HKEY hKey=NULL;
	LPCSTR lpFile=NULL;
	DWORD dwFlags=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = RegRestoreKeyA (hKey,lpFile,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegRestoreKeyA (hKey,lpFile,dwFlags);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
