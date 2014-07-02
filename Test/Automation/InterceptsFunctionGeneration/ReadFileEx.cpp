#include "common.h"


BOOL My_ReadFileEx()
{
	HANDLE hFile=NULL;
	LPVOID lpBuffer=NULL;
	DWORD nNumberOfBytesToRead=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = ReadFileEx (hFile,lpBuffer,nNumberOfBytesToRead,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = ReadFileEx (hFile,lpBuffer,nNumberOfBytesToRead,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
