#include "common.h"


BOOL My_GetVolumeInformationA()
{
	LPCSTR lpRootPathName=NULL;
	LPSTR lpVolumeNameBuffer=NULL;
	DWORD nVolumeNameSize=NULL;
	LPDWORD lpVolumeSerialNumber=NULL;
	LPDWORD lpMaximumComponentLength=NULL;
	LPDWORD lpFileSystemFlags=NULL;
	LPSTR lpFileSystemNameBuffer=NULL;
	DWORD nFileSystemNameSize=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetVolumeInformationA (lpRootPathName,lpVolumeNameBuffer,nVolumeNameSize,lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,lpFileSystemNameBuffer,nFileSystemNameSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetVolumeInformationA (lpRootPathName,lpVolumeNameBuffer,nVolumeNameSize,lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,lpFileSystemNameBuffer,nFileSystemNameSize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
