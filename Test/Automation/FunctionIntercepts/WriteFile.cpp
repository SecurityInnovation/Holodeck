#include "common.h"


BOOL My_WriteFile()
{
	HANDLE hFile=NULL;
	LPCVOID lpBuffer=NULL;
	DWORD nNumberOfBytesToWrite=NULL;
	LPDWORD lpNumberOfBytesWritten=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WriteFile (hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WriteFile (hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
