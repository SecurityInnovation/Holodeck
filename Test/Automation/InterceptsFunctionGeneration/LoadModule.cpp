#include "common.h"


BOOL My_LoadModule()
{
	LPCSTR lpModuleName=NULL;
	LPVOID lpParameterBlock=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = LoadModule (lpModuleName,lpParameterBlock);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = LoadModule (lpModuleName,lpParameterBlock);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
