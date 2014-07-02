#include "common.h"


BOOL My_SystemTimeToTzSpecificLocalTime()
{
	LPTIME_ZONE_INFORMATION lpTimeZoneInformation=NULL;
	LPSYSTEMTIME lpUniversalTime=NULL;
	LPSYSTEMTIME lpLocalTime=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = SystemTimeToTzSpecificLocalTime (lpTimeZoneInformation,lpUniversalTime,lpLocalTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = SystemTimeToTzSpecificLocalTime (lpTimeZoneInformation,lpUniversalTime,lpLocalTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
