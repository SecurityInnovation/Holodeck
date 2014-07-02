#include "common.h"


BOOL My_CopyFileExA()
{
	LPCSTR lpExistingFileName=NULL;
	LPCSTR lpNewFileName=NULL;
	LPPROGRESS_ROUTINE lpProgressRoutine=NULL;
	LPVOID lpData=NULL;
	LPBOOL pbCancel=NULL;
	DWORD dwCopyFlags=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CopyFileExA (lpExistingFileName,lpNewFileName,lpProgressRoutine,lpData,pbCancel,dwCopyFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CopyFileExA (lpExistingFileName,lpNewFileName,lpProgressRoutine,lpData,pbCancel,dwCopyFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
