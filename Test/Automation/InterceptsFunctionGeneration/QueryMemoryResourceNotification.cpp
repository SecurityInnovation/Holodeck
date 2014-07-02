#include "common.h"


BOOL My_QueryMemoryResourceNotification()
{
	HANDLE ResourceNotificationHandle=NULL;
	PBOOL ResourceState=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = QueryMemoryResourceNotification (ResourceNotificationHandle,ResourceState);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = QueryMemoryResourceNotification (ResourceNotificationHandle,ResourceState);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
