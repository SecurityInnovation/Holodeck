#include "common.h"


BOOL My_CreateFileA()
{
	LPCSTR lpFileName=NULL;
	DWORD dwDesiredAccess=NULL;
	DWORD dwShareMode=NULL;
	LPSECURITY_ATTRIBUTES lpSecurityAttibutes=NULL;
	DWORD dwCreationDisposition=NULL;
	DWORD dwFlagsAndAttributes=NULL;
	HANDLE hTemplateFile=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = CreateFileA (lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttibutes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateFileA (lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttibutes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
