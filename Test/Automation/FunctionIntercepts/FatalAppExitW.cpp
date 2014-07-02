#include "common.h"


BOOL My_FatalAppExitW()
{
	UINT uAction=NULL;
	LPCWSTR lpMessageText=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	FatalAppExitW (uAction,lpMessageText);
	error_Real = GetLastError();
	enableInterception();
	FatalAppExitW (uAction,lpMessageText);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return (error_Real == error_Intercepted);
}
