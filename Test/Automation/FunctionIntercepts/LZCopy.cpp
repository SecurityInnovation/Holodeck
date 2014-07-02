#include "common.h"


BOOL My_LZCopy()
{
	INT hfSource=NULL;
	INT hfDest=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	LZCopy (hfSource,hfDest);
	error_Real = GetLastError();
	enableInterception();
	LZCopy (hfSource,hfDest);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
