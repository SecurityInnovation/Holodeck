#include "common.h"


BOOL My_BackupWrite()
{
	HANDLE hFile=NULL;
	LPBYTE lpBuffer=NULL;
	DWORD nNumberofBytesToWrite=NULL;
	LPDWORD lpNumberOfBytesWritten=NULL;
	BOOL bAbort=NULL;
	BOOL bProcessSecurity=NULL;
	LPVOID * lpContext=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = BackupWrite (hFile,lpBuffer,nNumberofBytesToWrite,lpNumberOfBytesWritten,bAbort,bProcessSecurity,lpContext);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = BackupWrite (hFile,lpBuffer,nNumberofBytesToWrite,lpNumberOfBytesWritten,bAbort,bProcessSecurity,lpContext);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
