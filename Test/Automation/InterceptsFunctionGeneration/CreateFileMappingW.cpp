#include "common.h"


BOOL My_CreateFileMappingW()
{
	HANDLE hFile=NULL;
	LPSECURITY_ATTRIBUTES lpFileMappingAttributes=NULL;
	DWORD flProtect=NULL;
	DWORD dwMaximumSizeHigh=NULL;
	DWORD dwMaximumSizeLow=NULL;
	LPCWSTR lpName=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateFileMappingW (hFile,lpFileMappingAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateFileMappingW (hFile,lpFileMappingAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
