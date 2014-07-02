#include "common.h"


BOOL My_HeapAlloc()
{
	HANDLE hHeap=NULL;
	DWORD dwFlags=NULL;
	SIZE_T dwBytes=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = HeapAlloc (hHeap,dwFlags,dwBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapAlloc (hHeap,dwFlags,dwBytes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
