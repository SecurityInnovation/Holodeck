#include "common.h"


BOOL My_GetFileSecurityW()
{
	LPCWSTR lpFileName=NULL;
	SECURITY_INFORMATION RequestedInformation=NULL;
	PSECURITY_DESCRIPTOR pSecurityDescriptor=NULL;
	DWORD nLength=NULL;
	LPDWORD lpnLengthNeeded=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetFileSecurityW (lpFileName,RequestedInformation,pSecurityDescriptor,nLength,lpnLengthNeeded);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetFileSecurityW (lpFileName,RequestedInformation,pSecurityDescriptor,nLength,lpnLengthNeeded);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
