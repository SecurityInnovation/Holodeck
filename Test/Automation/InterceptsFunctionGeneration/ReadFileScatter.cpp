#include "common.h"


BOOL My_ReadFileScatter()
{
	HANDLE hFile=NULL;
	FILE_SEGMENT_ELEMENT aSegmentArray[50];
	DWORD nNumberOfBytesToRead=NULL;
	LPDWORD lpReserved=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = ReadFileScatter (hFile,aSegmentArray,nNumberOfBytesToRead,lpReserved,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = ReadFileScatter (hFile,aSegmentArray,nNumberOfBytesToRead,lpReserved,lpOverlapped);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
