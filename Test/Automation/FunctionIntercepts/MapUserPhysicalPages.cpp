#include "common.h"


BOOL My_MapUserPhysicalPages()
{
	PVOID VirtualAddress=NULL;
	ULONG_PTR NumberOfPages=NULL;
	PULONG_PTR PageArray=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = MapUserPhysicalPages (VirtualAddress,NumberOfPages,PageArray);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = MapUserPhysicalPages (VirtualAddress,NumberOfPages,PageArray);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
