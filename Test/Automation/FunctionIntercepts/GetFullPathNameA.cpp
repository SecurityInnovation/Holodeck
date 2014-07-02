#include "common.h"


BOOL My_GetFullPathNameA()
{
	LPCSTR lpFileName=NULL;
	DWORD nBufferLength=NULL;
	LPSTR lpBuffer=NULL;
	LPSTR * lpFilePart=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetFullPathNameA (lpFileName,nBufferLength,lpBuffer,lpFilePart);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetFullPathNameA (lpFileName,nBufferLength,lpBuffer,lpFilePart);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
