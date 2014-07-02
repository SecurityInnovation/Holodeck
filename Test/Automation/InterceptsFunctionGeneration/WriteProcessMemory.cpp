#include "common.h"


BOOL My_WriteProcessMemory()
{
	HANDLE hProcess=NULL;
	LPVOID lpBaseAddress=NULL;
	LPCVOID lpBuffer=NULL;
	SIZE_T nSize=NULL;
	SIZE_T * lpNumberOfBytesWritten=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WriteProcessMemory (hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesWritten);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WriteProcessMemory (hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesWritten);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
