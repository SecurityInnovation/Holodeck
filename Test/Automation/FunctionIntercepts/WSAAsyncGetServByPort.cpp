#include "common.h"


BOOL My_WSAAsyncGetServByPort()
{
	HWND hWnd=NULL;
	u_int wMsg=NULL;
	int port=NULL;
	const char FAR * proto=NULL;
	char FAR * buf=NULL;
	int buflen=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSAAsyncGetServByPort (hWnd,wMsg,port,proto,buf,buflen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAAsyncGetServByPort (hWnd,wMsg,port,proto,buf,buflen);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
