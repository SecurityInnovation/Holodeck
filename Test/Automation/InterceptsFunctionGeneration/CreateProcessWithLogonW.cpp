#include "common.h"


BOOL My_CreateProcessWithLogonW()
{
	LPCWSTR lpUsername=NULL;
	LPCWSTR lpDomain=NULL;
	LPCWSTR lpPassword=NULL;
	DWORD dwLogonFlags=NULL;
	LPCWSTR lpApplicationName=NULL;
	LPWSTR lpCommandLine=NULL;
	DWORD dwCreationFlags=NULL;
	LPVOID lpEnvironment=NULL;
	LPCWSTR lpCurrentDirectory=NULL;
	LPSTARTUPINFOW lpStartupInfo=NULL;
	LPPROCESS_INFORMATION lpProcessInformation=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateProcessWithLogonW (lpUsername,lpDomain,lpPassword,dwLogonFlags,lpApplicationName,lpCommandLine,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateProcessWithLogonW (lpUsername,lpDomain,lpPassword,dwLogonFlags,lpApplicationName,lpCommandLine,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
