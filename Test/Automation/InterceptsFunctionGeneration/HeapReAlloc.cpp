#include "common.h"


BOOL My_HeapReAlloc()
{
	HANDLE hHeap=NULL;
	DWORD dwFlags=NULL;
	LPVOID lpMem=NULL;
	SIZE_T dwBytes=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = HeapReAlloc (hHeap,dwFlags,lpMem,dwBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapReAlloc (hHeap,dwFlags,lpMem,dwBytes);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
