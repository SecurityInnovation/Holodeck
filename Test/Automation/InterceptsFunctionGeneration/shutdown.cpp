#include "common.h"


BOOL My_shutdown()
{
	SOCKET s=NULL;
	int how=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = shutdown (s,how);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = shutdown (s,how);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
