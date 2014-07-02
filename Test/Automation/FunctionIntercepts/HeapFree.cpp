#include "common.h"


BOOL My_HeapFree()
{
	HANDLE hHeap=NULL;
	DWORD dwFlags=NULL;
	LPVOID lpMem=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = HeapFree (hHeap,dwFlags,lpMem);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapFree (hHeap,dwFlags,lpMem);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
