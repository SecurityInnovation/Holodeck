#include "common.h"


BOOL My_RegNotifyChangeKeyValue()
{
	HKEY hKey=NULL;
	BOOL bWatchSubtree=NULL;
	DWORD dwNotifyFilter=NULL;
	HANDLE hEvent=NULL;
	BOOL fAsynchronous=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegNotifyChangeKeyValue (hKey,bWatchSubtree,dwNotifyFilter,hEvent,fAsynchronous);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegNotifyChangeKeyValue (hKey,bWatchSubtree,dwNotifyFilter,hEvent,fAsynchronous);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
