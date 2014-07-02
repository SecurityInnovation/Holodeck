#include "common.h"


BOOL My_recv()
{
	SOCKET s=NULL;
	char FAR * buf=NULL;
	int len=NULL;
	int flags=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = recv (s,buf,len,flags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = recv (s,buf,len,flags);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
