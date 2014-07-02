#include "common.h"


BOOL My_WSADuplicateSocketW()
{
	SOCKET s=NULL;
	DWORD dwProcessId=NULL;
	LPWSAPROTOCOL_INFOW lpProtocolInfo=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSADuplicateSocketW (s,dwProcessId,lpProtocolInfo);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSADuplicateSocketW (s,dwProcessId,lpProtocolInfo);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
