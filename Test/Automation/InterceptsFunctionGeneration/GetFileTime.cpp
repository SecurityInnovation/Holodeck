#include "common.h"


BOOL My_GetFileTime()
{
	HANDLE hFile=NULL;
	LPFILETIME lpCreationTime=NULL;
	LPFILETIME lpLastAccessTime=NULL;
	LPFILETIME lpLastWriteTime=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetFileTime (hFile,lpCreationTime,lpLastAccessTime,lpLastWriteTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetFileTime (hFile,lpCreationTime,lpLastAccessTime,lpLastWriteTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
