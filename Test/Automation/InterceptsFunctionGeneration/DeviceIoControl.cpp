#include "common.h"


BOOL My_DeviceIoControl()
{
	HANDLE hDevice=NULL;
	DWORD dwIoControlCode=NULL;
	LPVOID lpInBuffer=NULL;
	DWORD nInBufferSize=NULL;
	LPVOID lpOutBuffer=NULL;
	DWORD nOutBufferSize=NULL;
	LPDWORD lpBytesReturned=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = DeviceIoControl (hDevice,dwIoControlCode,lpInBuffer,nInBufferSize,lpOutBuffer,nOutBufferSize,lpBytesReturned,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = DeviceIoControl (hDevice,dwIoControlCode,lpInBuffer,nInBufferSize,lpOutBuffer,nOutBufferSize,lpBytesReturned,lpOverlapped);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
