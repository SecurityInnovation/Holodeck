#include "common.h"


BOOL My_listen()
{
	SOCKET s=NULL;
	int backlog=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = listen (s,backlog);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = listen (s,backlog);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
