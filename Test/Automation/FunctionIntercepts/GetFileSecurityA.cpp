#include "common.h"


BOOL My_GetFileSecurityA()
{
	LPCSTR lpFileName=NULL;
	SECURITY_INFORMATION RequestedInformation=NULL;
	PSECURITY_DESCRIPTOR pSecurityDescriptor=NULL;
	DWORD nLength=NULL;
	LPDWORD lpnLengthNeeded=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetFileSecurityA (lpFileName,RequestedInformation,pSecurityDescriptor,nLength,lpnLengthNeeded);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetFileSecurityA (lpFileName,RequestedInformation,pSecurityDescriptor,nLength,lpnLengthNeeded);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
