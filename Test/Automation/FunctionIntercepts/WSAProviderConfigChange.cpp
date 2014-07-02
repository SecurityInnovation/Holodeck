#include "common.h"


BOOL My_WSAProviderConfigChange()
{
	LPHANDLE lpNotificationHandle=NULL;
	LPWSAOVERLAPPED lpOverlapped=NULL;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL;
	INT returnVal_Real = NULL;
	INT returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSAProviderConfigChange (lpNotificationHandle,lpOverlapped,lpCompletionRoutine);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSAProviderConfigChange (lpNotificationHandle,lpOverlapped,lpCompletionRoutine);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
