#include "common.h"


BOOL My_LZClose()
{
	INT hFile=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	LZClose (hFile);
	error_Real = GetLastError();
	enableInterception();
	LZClose (hFile);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
