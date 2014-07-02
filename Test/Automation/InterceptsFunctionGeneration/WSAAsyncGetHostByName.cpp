#include "common.h"


BOOL My_WSAAsyncGetHostByName()
{
	HWND hWnd=NULL;
	u_int wMsg=NULL;
	const char FAR * name=NULL;
	char FAR * buf=NULL;
	int buflen=NULL;
	HANDLE returnVal_Real = NULL;
	HANDLE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAAsyncGetHostByName (hWnd,wMsg,name,buf,buflen);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAAsyncGetHostByName (hWnd,wMsg,name,buf,buflen);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}