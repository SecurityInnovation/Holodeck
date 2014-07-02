#include "common.h"


BOOL My_WSAStartup()
{
	WORD wVersionRequested=NULL;
	LPWSADATA lpWSAData=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAStartup (wVersionRequested,lpWSAData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAStartup (wVersionRequested,lpWSAData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
