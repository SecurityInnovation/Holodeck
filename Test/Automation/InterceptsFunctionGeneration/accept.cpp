#include "common.h"


BOOL My_accept()
{
	SOCKET s=NULL;
	struct sockaddr FAR * addr=NULL;
	int FAR * addrlen=NULL;
	SOCKET returnVal_Real = NULL;
	SOCKET returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = accept (s,addr,addrlen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = accept (s,addr,addrlen);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
