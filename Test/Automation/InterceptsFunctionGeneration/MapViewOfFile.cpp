#include "common.h"


BOOL My_MapViewOfFile()
{
	HANDLE hFileMappingObject=NULL;
	DWORD dwDesiredAccess=NULL;
	DWORD dwFileOffsetHigh=NULL;
	DWORD dwFileOffsetLow=NULL;
	SIZE_T dwNumberOfBytesToMap=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = MapViewOfFile (hFileMappingObject,dwDesiredAccess,dwFileOffsetHigh,dwFileOffsetLow,dwNumberOfBytesToMap);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = MapViewOfFile (hFileMappingObject,dwDesiredAccess,dwFileOffsetHigh,dwFileOffsetLow,dwNumberOfBytesToMap);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
