#include "common.h"


BOOL My_CopyLZFile()
{
	INT hfSource=NULL;
	INT hfDest=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CopyLZFile (hfSource,hfDest);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CopyLZFile (hfSource,hfDest);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
