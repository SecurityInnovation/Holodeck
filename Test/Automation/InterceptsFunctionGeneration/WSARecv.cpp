#include "common.h"


BOOL My_WSARecv()
{
	SOCKET s=NULL;
	LPWSABUF lpBuffers=NULL;
	DWORD dwBufferCount=NULL;
	LPDWORD lpNumberOfBytesRecvd=NULL;
	LPDWORD lpFlags=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSARecv (s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSARecv (s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
