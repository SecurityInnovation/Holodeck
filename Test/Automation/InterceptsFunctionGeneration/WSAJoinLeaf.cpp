#include "common.h"


BOOL My_WSAJoinLeaf()
{
	SOCKET s=NULL;
	const struct sockaddr FAR * name=NULL;
	int namelen=NULL;
	LPWSABUF lpCallerData=NULL;
	LPWSABUF lpCalleeData=NULL;
	LPQOS lpSQOS=NULL;
	LPQOS lpGQOS=NULL;
	DWORD dwFlags=NULL;
	SOCKET returnVal_Real = NULL;
	SOCKET returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAJoinLeaf (s,name,namelen,lpCallerData,lpCalleeData,lpSQOS,lpGQOS,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAJoinLeaf (s,name,namelen,lpCallerData,lpCalleeData,lpSQOS,lpGQOS,dwFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
