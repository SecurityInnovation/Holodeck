#include "common.h"


BOOL My_VirtualQueryEx()
{
	HANDLE hProcess=NULL;
	LPCVOID lpAddress=NULL;
	PMEMORY_BASIC_INFORMATION lpBuffer=NULL;
	SIZE_T dwLength=NULL;
	SIZE_T returnVal_Real = NULL;
	SIZE_T returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = VirtualQueryEx (hProcess,lpAddress,lpBuffer,dwLength);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = VirtualQueryEx (hProcess,lpAddress,lpBuffer,dwLength);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
