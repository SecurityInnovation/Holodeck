#include "common.h"


BOOL My_SearchPathA()
{
	LPCSTR lpPath=NULL;
	LPCSTR lpFileName=NULL;
	LPCSTR lpExtension=NULL;
	DWORD nBufferLength=NULL;
	LPSTR lpBuffer=NULL;
	LPSTR * lpFilePart=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SearchPathA (lpPath,lpFileName,lpExtension,nBufferLength,lpBuffer,lpFilePart);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SearchPathA (lpPath,lpFileName,lpExtension,nBufferLength,lpBuffer,lpFilePart);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
