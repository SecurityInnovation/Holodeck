#include "common.h"


BOOL My_GetFileInformationByHandle()
{
	HANDLE hFile=NULL;
	LPBY_HANDLE_FILE_INFORMATION lpFileInformation=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetFileInformationByHandle (hFile,lpFileInformation);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetFileInformationByHandle (hFile,lpFileInformation);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
