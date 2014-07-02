#include "common.h"


BOOL My_HeapCreate()
{
	DWORD flOptions=NULL;
	SIZE_T dwInitialSize=NULL;
	SIZE_T dwMaximumSize=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = HeapCreate (flOptions,dwInitialSize,dwMaximumSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = HeapCreate (flOptions,dwInitialSize,dwMaximumSize);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	//return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
	return (( HeapValidate( returnVal_Real, NULL, NULL ) == HeapValidate( returnVal_Intercepted, NULL, NULL )) && (error_Real == error_Intercepted));
}
