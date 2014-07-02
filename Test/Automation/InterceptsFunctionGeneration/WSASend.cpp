#include "common.h"


BOOL My_WSASend()
{
	SOCKET s=NULL;
	LPWSABUF lpBuffers=NULL;
	DWORD dwBufferCount=NULL;
	LPDWORD lpNumberOfBytesSent=NULL;
	DWORD dwFlags=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSASend (s,lpBuffers,dwBufferCount,lpNumberOfBytesSent,dwFlags,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSASend (s,lpBuffers,dwBufferCount,lpNumberOfBytesSent,dwFlags,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
