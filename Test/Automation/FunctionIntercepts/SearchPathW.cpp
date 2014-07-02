#include "common.h"


BOOL My_SearchPathW()
{
	LPCWSTR lpPath=NULL;
	LPCWSTR lpFileName=NULL;
	LPCWSTR lpExtension=NULL;
	DWORD nBufferLength=NULL;
	LPWSTR lpBuffer=NULL;
	LPWSTR * lpFilePart=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = SearchPathW (lpPath,lpFileName,lpExtension,nBufferLength,lpBuffer,lpFilePart);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SearchPathW (lpPath,lpFileName,lpExtension,nBufferLength,lpBuffer,lpFilePart);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
