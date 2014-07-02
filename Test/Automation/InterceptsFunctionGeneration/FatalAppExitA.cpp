#include "common.h"


BOOL My_FatalAppExitA()
{
	UINT uAction=NULL;
	LPCSTR lpMessageText=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	FatalAppExitA (uAction,lpMessageText);
	error_Real = GetLastError();
	enableInterception();
	FatalAppExitA (uAction,lpMessageText);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
