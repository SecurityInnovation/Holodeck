#include "common.h"


BOOL My_QueryUsersOnEncryptedFile()
{
	LPCWSTR lpFileName=NULL;
	PENCRYPTION_CERTIFICATE_HASH_LIST * pUsers=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = QueryUsersOnEncryptedFile (lpFileName,pUsers);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = QueryUsersOnEncryptedFile (lpFileName,pUsers);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
