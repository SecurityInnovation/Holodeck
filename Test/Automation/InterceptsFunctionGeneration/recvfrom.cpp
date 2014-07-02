#include "common.h"


BOOL My_recvfrom()
{
	SOCKET s=NULL;
	char FAR * buf=NULL;
	int len=NULL;
	int flags=NULL;
	struct sockaddr FAR * from=NULL;
	int FAR * fromlen=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = recvfrom (s,buf,len,flags,from,fromlen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = recvfrom (s,buf,len,flags,from,fromlen);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
