#include "common.h"


BOOL My_GetProcAddress()
{
	HMODULE hModule=NULL;
	LPCSTR lpProcName=NULL;
	FARPROC returnVal_Real = NULL;
	FARPROC returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetProcAddress (hModule,lpProcName);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetProcAddress (hModule,lpProcName);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
