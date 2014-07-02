#include "common.h"


BOOL My_WSACancelAsyncRequest()
{
	HANDLE hAsyncTaskHandle=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSACancelAsyncRequest (hAsyncTaskHandle);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSACancelAsyncRequest (hAsyncTaskHandle);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
