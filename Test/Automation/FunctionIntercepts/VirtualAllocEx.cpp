#include "common.h"


BOOL My_VirtualAllocEx()
{
	HANDLE hProcess=NULL;
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
	returnVal_Real = VirtualAllocEx (hProcess,lpAddress,dwSize,fAllocationType,flProtect);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = VirtualAllocEx (hProcess,lpAddress,dwSize,fAllocationType,flProtect);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
