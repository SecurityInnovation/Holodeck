#include "common.h"


BOOL My_DeleteFiber()
{
	LPVOID lpFiber=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	DeleteFiber (lpFiber);
	error_Real = GetLastError();
	enableInterception();
	DeleteFiber (lpFiber);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
