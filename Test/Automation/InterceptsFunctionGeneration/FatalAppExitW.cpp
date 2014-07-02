#include "common.h"


BOOL My_FatalAppExitW()
{
	UINT uAction=NULL;
	LPCWSTR lpMessageText=NULL;
	
	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	FatalAppExitW (uAction,lpMessageText);
	error_Real = GetLastError();
	enableInterception();
	FatalAppExitW (uAction,lpMessageText);
	error_Intercepted = GetLastError();
	return (error_Real == error_Intercepted);
}
