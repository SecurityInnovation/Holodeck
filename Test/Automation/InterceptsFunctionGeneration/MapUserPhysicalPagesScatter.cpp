#include "common.h"


BOOL My_MapUserPhysicalPagesScatter()
{
	PVOID * VirtualAddress=NULL;
	ULONG_PTR NumberOfPages=NULL;
	PULONG_PTR PageArray=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = MapUserPhysicalPagesScatter (VirtualAddress,NumberOfPages,PageArray);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = MapUserPhysicalPagesScatter (VirtualAddress,NumberOfPages,PageArray);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
