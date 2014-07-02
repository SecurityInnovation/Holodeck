#include "common.h"


BOOL My_CreateFiberEx()
{
	SIZE_T dwStackCommitSize=NULL;
	SIZE_T dwStackReserveSize=NULL;
	DWORD dwFlags=NULL;
	LPFIBER_START_ROUTINE lpStartAddress=NULL;
	LPVOID lpParameter=NULL;
	LPVOID returnVal_Real = NULL;
	LPVOID returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateFiberEx (dwStackCommitSize,dwStackReserveSize,dwFlags,lpStartAddress,lpParameter);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateFiberEx (dwStackCommitSize,dwStackReserveSize,dwFlags,lpStartAddress,lpParameter);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
