#include "common.h"


BOOL My_VirtualProtectEx()
{
	HANDLE hProcess=NULL;
	LPVOID lpAddress=NULL;
	SIZE_T dwSize=NULL;
	DWORD flNewProtect=NULL;
	PDWORD lpflOldProtect=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = VirtualProtectEx (hProcess,lpAddress,dwSize,flNewProtect,lpflOldProtect);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = VirtualProtectEx (hProcess,lpAddress,dwSize,flNewProtect,lpflOldProtect);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
