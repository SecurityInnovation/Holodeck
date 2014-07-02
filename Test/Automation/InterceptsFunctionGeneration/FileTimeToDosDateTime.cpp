#include "common.h"


BOOL My_FileTimeToDosDateTime()
{
	CONST FILETIME * lpFileTime=NULL;
	LPWORD lpFatDate=NULL;
	LPWORD lpFatTime=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = FileTimeToDosDateTime (lpFileTime,lpFatDate,lpFatTime);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = FileTimeToDosDateTime (lpFileTime,lpFatDate,lpFatTime);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
