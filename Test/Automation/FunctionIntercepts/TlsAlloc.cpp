#include "common.h"


BOOL My_TlsAlloc()
{
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = TlsAlloc ();
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = TlsAlloc ();
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((( returnVal_Real != NULL ) == ( returnVal_Intercepted != NULL )) && (error_Real == error_Intercepted));
}
