#include "common.h"


BOOL My_WriteFileEx()
{
	HANDLE hFile=NULL;
	LPCVOID lpBuffer=NULL;
	DWORD nNumberOfBytesToWrite=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WriteFileEx (hFile,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WriteFileEx (hFile,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
