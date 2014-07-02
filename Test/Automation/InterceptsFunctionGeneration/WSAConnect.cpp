#include "common.h"


BOOL My_WSAConnect()
{
	SOCKET s=NULL;
	const struct sockaddr FAR * name=NULL;
	int namelen=NULL;
	LPWSABUF lpCallerData=NULL;
	LPWSABUF lpCalleeData=NULL;
	LPQOS lpSQOS=NULL;
	LPQOS lpGQOS=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAConnect (s,name,namelen,lpCallerData,lpCalleeData,lpSQOS,lpGQOS);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAConnect (s,name,namelen,lpCallerData,lpCalleeData,lpSQOS,lpGQOS);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
