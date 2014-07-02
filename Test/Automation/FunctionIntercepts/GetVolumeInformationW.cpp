#include "common.h"


BOOL My_GetVolumeInformationW()
{
	LPCWSTR lpRootPathName=NULL;
	LPWSTR lpVolumeNameBuffer=NULL;
	DWORD nVolumeNameSize=NULL;
	LPDWORD lpVolumeSerialNumber=NULL;
	LPDWORD lpMaximumComponentLength=NULL;
	LPDWORD lpFileSystemFlags=NULL;
	LPWSTR lpFileSystemNameBuffer=NULL;
	DWORD nFileSystemNameSize=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = GetVolumeInformationW (lpRootPathName,lpVolumeNameBuffer,nVolumeNameSize,lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,lpFileSystemNameBuffer,nFileSystemNameSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetVolumeInformationW (lpRootPathName,lpVolumeNameBuffer,nVolumeNameSize,lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,lpFileSystemNameBuffer,nFileSystemNameSize);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
