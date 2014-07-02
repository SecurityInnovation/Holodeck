#include "common.h"


BOOL My_RemoveUsersFromEncryptedFile()
{
	LPCWSTR lpFileName=NULL;
	PENCRYPTION_CERTIFICATE_HASH_LIST pHashes=NULL;
	DWORD returnVal_Real = NULL;
	DWORD returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = RemoveUsersFromEncryptedFile (lpFileName,pHashes);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = RemoveUsersFromEncryptedFile (lpFileName,pHashes);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
