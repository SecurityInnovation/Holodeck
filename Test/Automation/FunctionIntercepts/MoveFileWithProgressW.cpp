#include "common.h"


BOOL My_MoveFileWithProgressW()
{
	LPCWSTR lpExistingFileName=NULL;
	LPCWSTR lpNewFileName=NULL;
	LPPROGRESS_ROUTINE lpProgressRoutine=NULL;
	LPVOID lpData=NULL;
	DWORD dwFlags=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = MoveFileWithProgressW (lpExistingFileName,lpNewFileName,lpProgressRoutine,lpData,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = MoveFileWithProgressW (lpExistingFileName,lpNewFileName,lpProgressRoutine,lpData,dwFlags);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
