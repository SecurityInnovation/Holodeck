#include "common.h"


BOOL My_GetWriteWatch()
{
	DWORD dwFlags=NULL;
	PVOID lpBaseAddress=NULL;
	SIZE_T dwRegionSize=NULL;
	PVOID * lpAddresses=NULL;
	PULONG_PTR lpdwCount=NULL;
	PULONG lpdwGranularity=NULL;
	UINT returnVal_Real = NULL;
	UINT returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetWriteWatch (dwFlags,lpBaseAddress,dwRegionSize,lpAddresses,lpdwCount,lpdwGranularity);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetWriteWatch (dwFlags,lpBaseAddress,dwRegionSize,lpAddresses,lpdwCount,lpdwGranularity);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
