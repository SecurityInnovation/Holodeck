#include "common.h"


BOOL My_FindFirstFileExA()
{
	LPCSTR lpFileName=NULL;
	FINDEX_INFO_LEVELS fInfoLevelId;
	LPVOID lpFindFileData=NULL;
	FINDEX_SEARCH_OPS fSearchOp;
	LPVOID lpSearchFilter=NULL;
	DWORD dwAdditionalFlags=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = FindFirstFileExA (lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FindFirstFileExA (lpFileName,fInfoLevelId,lpFindFileData,fSearchOp,lpSearchFilter,dwAdditionalFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
