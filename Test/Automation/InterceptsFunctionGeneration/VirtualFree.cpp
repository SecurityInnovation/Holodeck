#include "common.h"


BOOL My_VirtualFree()
{
	LPVOID lpAddress=NULL;
	SIZE_T dwSize=NULL;
	DWORD dwFreeType=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = VirtualFree (lpAddress,dwSize,dwFreeType);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = VirtualFree (lpAddress,dwSize,dwFreeType);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
