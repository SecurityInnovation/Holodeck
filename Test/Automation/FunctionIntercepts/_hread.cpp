#include "common.h"


BOOL My__hread()
{
	HFILE hFile=NULL;
	LPVOID lpBuffer=NULL;
	long lBytes=NULL;
	long returnVal_Real = NULL;
	long returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = _hread (hFile,lpBuffer,lBytes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = _hread (hFile,lpBuffer,lBytes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
