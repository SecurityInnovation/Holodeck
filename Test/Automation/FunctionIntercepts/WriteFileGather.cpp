#include "common.h"


BOOL My_WriteFileGather()
{
	HANDLE hFile=NULL;
	FILE_SEGMENT_ELEMENT aSegmentArray[50];
	DWORD nNumberOfBytesToWrite=NULL;
	LPDWORD lpReserved=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WriteFileGather (hFile,aSegmentArray,nNumberOfBytesToWrite,lpReserved,lpOverlapped);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WriteFileGather (hFile,aSegmentArray,nNumberOfBytesToWrite,lpReserved,lpOverlapped);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
