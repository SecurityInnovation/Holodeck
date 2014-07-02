#include "common.h"


BOOL My_WSAEventSelect()
{
	SOCKET s=NULL;
	WSAEVENT hEventObject=NULL;
	long lNetworkEvents=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSAEventSelect (s,hEventObject,lNetworkEvents);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAEventSelect (s,hEventObject,lNetworkEvents);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
