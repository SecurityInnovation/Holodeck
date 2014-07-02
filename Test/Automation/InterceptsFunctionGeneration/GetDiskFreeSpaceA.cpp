#include "common.h"


BOOL My_GetDiskFreeSpaceA()
{
	LPCSTR lpRootPathName=NULL;
	LPDWORD lpSectorsPerCluster=NULL;
	LPDWORD lpBytesPerSector=NULL;
	LPDWORD lpNumberOfFreeClusters=NULL;
	LPDWORD lpTotalNumberOfClusters=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = GetDiskFreeSpaceA (lpRootPathName,lpSectorsPerCluster,lpBytesPerSector,lpNumberOfFreeClusters,lpTotalNumberOfClusters);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = GetDiskFreeSpaceA (lpRootPathName,lpSectorsPerCluster,lpBytesPerSector,lpNumberOfFreeClusters,lpTotalNumberOfClusters);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}