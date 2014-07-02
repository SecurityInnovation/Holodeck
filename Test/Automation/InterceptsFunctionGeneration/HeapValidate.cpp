#include "common.h"


BOOL My_HeapValidate()
{
	HANDLE hHeap=NULL;
	DWORD dwFlags=NULL;
	LPCVOID lpMem=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = HeapValidate (hHeap,dwFlags,lpMem);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapValidate (hHeap,dwFlags,lpMem);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
