#include "common.h"


BOOL My_MoveFileExA()
{
	LPCSTR lpExistingFileName=NULL;
	LPCSTR lpNewFileName=NULL;
	DWORD dwFlags=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = MoveFileExA (lpExistingFileName,lpNewFileName,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = MoveFileExA (lpExistingFileName,lpNewFileName,dwFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
