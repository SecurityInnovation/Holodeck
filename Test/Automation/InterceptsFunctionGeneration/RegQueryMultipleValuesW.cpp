#include "common.h"


BOOL My_RegQueryMultipleValuesW()
{
	HKEY hKey=NULL;
	PVALENTW val_list=NULL;
	DWORD num_vals=NULL;
	LPWSTR lpValueBuf=NULL;
	LPDWORD ldwTotsize=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegQueryMultipleValuesW (hKey,val_list,num_vals,lpValueBuf,ldwTotsize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegQueryMultipleValuesW (hKey,val_list,num_vals,lpValueBuf,ldwTotsize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
