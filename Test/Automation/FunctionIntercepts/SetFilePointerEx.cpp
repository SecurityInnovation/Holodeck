#include "common.h"


BOOL My_SetFilePointerEx()
{
	HANDLE hFile=NULL;
	LARGE_INTEGER lDistanceToMove=NULL;
	PLARGE_INTEGER lpNewFilePointer=NULL;
	DWORD dwMoveMethod=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = SetFilePointerEx (hFile,lDistanceToMove,lpNewFilePointer,dwMoveMethod);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetFilePointerEx (hFile,lDistanceToMove,lpNewFilePointer,dwMoveMethod);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught"); enableInterception(); }
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
