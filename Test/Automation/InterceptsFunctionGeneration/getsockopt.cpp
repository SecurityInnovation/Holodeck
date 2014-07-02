#include "common.h"


BOOL My_getsockopt()
{
	SOCKET s=NULL;
	int level=NULL;
	int optname=NULL;
	char FAR * optval=NULL;
	int FAR * optlen=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = getsockopt (s,level,optname,optval,optlen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = getsockopt (s,level,optname,optval,optlen);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
