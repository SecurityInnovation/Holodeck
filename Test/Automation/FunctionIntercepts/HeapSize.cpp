#include "common.h"


BOOL My_HeapSize()
{
	HANDLE hHeap=NULL;
	DWORD dwFlags=NULL;
	LPCVOID lpMem=NULL;
	SIZE_T returnVal_Real = NULL;
	SIZE_T returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = HeapSize (hHeap,dwFlags,lpMem);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapSize (hHeap,dwFlags,lpMem);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
