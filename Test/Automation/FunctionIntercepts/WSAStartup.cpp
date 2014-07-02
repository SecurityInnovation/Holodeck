#include "common.h"


BOOL My_WSAStartup()
{
	WORD wVersionRequested=NULL;
	LPWSADATA lpWSAData=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSAStartup (wVersionRequested,lpWSAData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAStartup (wVersionRequested,lpWSAData);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
