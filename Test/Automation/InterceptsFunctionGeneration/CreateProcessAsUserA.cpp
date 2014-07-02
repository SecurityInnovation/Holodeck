#include "common.h"


BOOL My_CreateProcessAsUserA()
{
	HANDLE hToken=NULL;
	LPCSTR lpApplicationName=NULL;
	LPSTR lpCommandLine=NULL;
	LPSECURITY_ATTRIBUTES lpProcessAttributes=NULL;
	LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL;
	BOOL bInheritHandles=NULL;
	DWORD dwCreationFlags=NULL;
	LPVOID lpEnvironment=NULL;
	LPCSTR lpCurrentDirectory=NULL;
	LPSTARTUPINFOA lpStartupInfo=NULL;
	LPPROCESS_INFORMATION lpProcessInformation=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = CreateProcessAsUserA (hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = CreateProcessAsUserA (hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
