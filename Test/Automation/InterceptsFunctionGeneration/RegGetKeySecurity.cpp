#include "common.h"


BOOL My_RegGetKeySecurity()
{
	HKEY hKey=NULL;
	SECURITY_INFORMATION SecurityInformation=NULL;
	PSECURITY_DESCRIPTOR pSecurityDescriptor=NULL;
	LPDWORD lpcbSecurityDescriptor=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegGetKeySecurity (hKey,SecurityInformation,pSecurityDescriptor,lpcbSecurityDescriptor);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegGetKeySecurity (hKey,SecurityInformation,pSecurityDescriptor,lpcbSecurityDescriptor);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
