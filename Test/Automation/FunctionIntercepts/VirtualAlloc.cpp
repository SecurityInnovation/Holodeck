#include "common.h"


BOOL My_VirtualAlloc()
{
	LPVOID lpAddress=NULL;
	SIZE_T dwSize=NULL;
	DWORD fAllocationType=NULL;
	DWORD flProtect=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = VirtualAlloc (lpAddress,dwSize,fAllocationType,flProtect);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = VirtualAlloc (lpAddress,dwSize,fAllocationType,flProtect);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
