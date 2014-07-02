#include "common.h"


BOOL My_WSAAccept()
{
	SOCKET s=NULL;
	struct sockaddr FAR * addr=NULL;
	LPINT addrlen=NULL;
	LPCONDITIONPROC lpfnCondition=NULL;
	DWORD_PTR dwCallbackData=NULL;
	SOCKET returnVal_Real = NULL;
	SOCKET returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAAccept (s,addr,addrlen,lpfnCondition,dwCallbackData);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAAccept (s,addr,addrlen,lpfnCondition,dwCallbackData);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
