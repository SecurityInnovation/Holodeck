#include "common.h"


BOOL My_ioctlsocket()
{
	SOCKET s=NULL;
	long cmd=NULL;
	u_long FAR * argp=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = ioctlsocket (s,cmd,argp);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = ioctlsocket (s,cmd,argp);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
