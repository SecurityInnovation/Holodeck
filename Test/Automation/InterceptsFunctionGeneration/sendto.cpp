#include "common.h"


BOOL My_sendto()
{
	SOCKET s=NULL;
	const char FAR * buf=NULL;
	int len=NULL;
	int flags=NULL;
	const struct sockaddr FAR * to=NULL;
	int tolen=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = sendto (s,buf,len,flags,to,tolen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = sendto (s,buf,len,flags,to,tolen);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
