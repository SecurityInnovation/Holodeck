#include "common.h"


BOOL My_LZSeek()
{
	INT hFile=NULL;
	LONG lOffset=NULL;
	INT iOrigin=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = LZSeek (hFile,lOffset,iOrigin);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LZSeek (hFile,lOffset,iOrigin);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
