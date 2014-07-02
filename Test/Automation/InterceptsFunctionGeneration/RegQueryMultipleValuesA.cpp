#include "common.h"


BOOL My_RegQueryMultipleValuesA()
{
	HKEY hKey=NULL;
	PVALENTA val_list=NULL;
	DWORD num_vals=NULL;
	LPSTR lpValueBuf=NULL;
	LPDWORD ldwTotsize=NULL;
	LONG returnVal_Real = NULL;
	LONG returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = RegQueryMultipleValuesA (hKey,val_list,num_vals,lpValueBuf,ldwTotsize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RegQueryMultipleValuesA (hKey,val_list,num_vals,lpValueBuf,ldwTotsize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
