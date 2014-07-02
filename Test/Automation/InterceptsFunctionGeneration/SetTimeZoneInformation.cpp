#include "common.h"


BOOL My_SetTimeZoneInformation()
{
	CONST TIME_ZONE_INFORMATION * lpTimeZoneInformation=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SetTimeZoneInformation (lpTimeZoneInformation);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SetTimeZoneInformation (lpTimeZoneInformation);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
