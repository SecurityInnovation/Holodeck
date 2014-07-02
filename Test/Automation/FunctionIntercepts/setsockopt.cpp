#include "common.h"


BOOL My_setsockopt()
{
	SOCKET s=NULL;
	int level=NULL;
	int optname=NULL;
	const char FAR * optval=NULL;
	int optlen=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = setsockopt (s,level,optname,optval,optlen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = setsockopt (s,level,optname,optval,optlen);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
