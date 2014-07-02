#include "common.h"


BOOL My_WSAAsyncSelect()
{
	SOCKET s=NULL;
	HWND hWnd=NULL;
	u_int wMsg=NULL;
	long lEvent=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSAAsyncSelect (s,hWnd,wMsg,lEvent);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAAsyncSelect (s,hWnd,wMsg,lEvent);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
