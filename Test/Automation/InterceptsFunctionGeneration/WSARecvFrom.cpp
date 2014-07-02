#include "common.h"


BOOL My_WSARecvFrom()
{
	SOCKET s=NULL;
	LPWSABUF lpBuffers=NULL;
	DWORD dwBufferCount=NULL;
	LPDWORD lpNumberOfBytesRecvd=NULL;
	LPDWORD lpFlags=NULL;
	struct sockaddr FAR * lpFrom=NULL;
	LPINT lpFromlen=NULL;
	LPWSAOVERLAPPED lpOverlapped=NULL;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSARecvFrom (s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpFrom,lpFromlen,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSARecvFrom (s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpFrom,lpFromlen,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
