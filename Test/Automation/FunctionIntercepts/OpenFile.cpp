#include "common.h"


BOOL My_OpenFile()
{
	LPCSTR lpFileName=NULL;
	LPOFSTRUCT lpReOpenBuff=NULL;
	UINT uStyle=NULL;
	HFILE returnVal_Real = NULL;
	HFILE returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = OpenFile (lpFileName,lpReOpenBuff,uStyle);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = OpenFile (lpFileName,lpReOpenBuff,uStyle);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
