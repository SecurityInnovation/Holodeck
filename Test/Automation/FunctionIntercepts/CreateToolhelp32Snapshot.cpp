#include "common.h"


BOOL My_CreateToolhelp32Snapshot()
{
	DWORD dwFlags=TH32CS_SNAPALL;
	DWORD th32ProcessID=-123;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = CreateToolhelp32Snapshot (dwFlags,th32ProcessID);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateToolhelp32Snapshot (dwFlags,th32ProcessID);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
