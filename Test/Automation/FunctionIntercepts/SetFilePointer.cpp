#include "common.h"


BOOL My_SetFilePointer()
{
	HANDLE hFile=NULL;
	LONG lDistanceToMove=NULL;
	PLONG lpDistanceToMoveHigh=NULL;
	DWORD dwMoveMethod=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = SetFilePointer (hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetFilePointer (hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
