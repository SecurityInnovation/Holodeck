#include "common.h"


BOOL My_CreateFiber()
{
	SIZE_T dwStackSize=NULL;
	LPFIBER_START_ROUTINE lpStartAddress=NULL;
	LPVOID lpParameter=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateFiber (dwStackSize,lpStartAddress,lpParameter);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateFiber (dwStackSize,lpStartAddress,lpParameter);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
