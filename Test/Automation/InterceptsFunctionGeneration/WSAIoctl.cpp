#include "common.h"


BOOL My_WSAIoctl()
{
	SOCKET s=NULL;
	DWORD dwIoControlCode=NULL;
	LPVOID lpvInBuffer=NULL;
	DWORD cbInBuffer=NULL;
	LPVOID lpvOutBuffer=NULL;
	DWORD cbOutBuffer=NULL;
	LPDWORD lpcbBytesReturned=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSAIoctl (s,dwIoControlCode,lpvInBuffer,cbInBuffer,lpvOutBuffer,cbOutBuffer,lpcbBytesReturned,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAIoctl (s,dwIoControlCode,lpvInBuffer,cbInBuffer,lpvOutBuffer,cbOutBuffer,lpcbBytesReturned,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
