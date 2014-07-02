#include "common.h"


BOOL My_socket()
{
	int af=NULL;
	int type=NULL;
	int protocol=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = socket (af,type,protocol);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = socket (af,type,protocol);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
