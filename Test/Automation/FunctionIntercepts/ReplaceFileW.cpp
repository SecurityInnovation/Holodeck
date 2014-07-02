#include "common.h"


BOOL My_ReplaceFileW()
{
	LPCWSTR lpReplacedFileName=NULL;
	LPCWSTR lpReplacementFileName=NULL;
	LPCWSTR lpBackupFileName=NULL;
	DWORD dwReplaceFlags=NULL;
	LPVOID lpExclude=NULL;
	LPVOID lpReserved=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = ReplaceFileW (lpReplacedFileName,lpReplacementFileName,lpBackupFileName,dwReplaceFlags,lpExclude,lpReserved);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = ReplaceFileW (lpReplacedFileName,lpReplacementFileName,lpBackupFileName,dwReplaceFlags,lpExclude,lpReserved);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
