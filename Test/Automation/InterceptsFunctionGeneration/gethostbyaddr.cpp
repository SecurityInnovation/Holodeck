#include "common.h"


BOOL My_gethostbyaddr()
{
	const char FAR * addr=NULL;
	int len=NULL;
	int type=NULL;
	struct hostent FAR * returnVal_Real = NULL;
	struct hostent FAR * returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = gethostbyaddr (addr,len,type);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = gethostbyaddr (addr,len,type);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
