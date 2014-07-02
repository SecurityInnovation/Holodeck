#include "common.h"


BOOL My_BackupSeek()
{
	HANDLE hFile=NULL;
	DWORD dwLowBytesToSeek=NULL;
	DWORD dwHighBytesToSeek=NULL;
	LPDWORD lpdwLowByteSeeked=NULL;
	LPDWORD lpdwHighByteSeeked=NULL;
	LPVOID * lpContext=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = BackupSeek (hFile,dwLowBytesToSeek,dwHighBytesToSeek,lpdwLowByteSeeked,lpdwHighByteSeeked,lpContext);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = BackupSeek (hFile,dwLowBytesToSeek,dwHighBytesToSeek,lpdwLowByteSeeked,lpdwHighByteSeeked,lpContext);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
