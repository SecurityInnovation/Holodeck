#include "common.h"


BOOL My_CreateThread()
{
	LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL;
	SIZE_T dwStackSize=NULL;
	LPTHREAD_START_ROUTINE lpStartAddress=NULL;
	LPVOID lpParameter=NULL;
	DWORD dwCreationFlags=NULL;
	LPDWORD lpThreadId=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateThread (lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateThread (lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
