#include "stdafx.h"


bool InsufficientMemory()
{	
	bool bSuccess = TRUE;

	if (!My_malloc(0, 8))
		bSuccess = FALSE;

	if (!My_LocalAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_LocalReAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_HeapCreate(0, 8))
		bSuccess = FALSE;

	if (!My_HeapAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_HeapReAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_VirtualAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_VirtualAllocEx(0, 8))
		bSuccess = FALSE;

	if (!My_GlobalAlloc(0, 8))
		bSuccess = FALSE;

	if (!My_CopyMemory(0, 8))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool InvalidAccessToLocation()
{	
	bool bSuccess = TRUE;

	if (!My_malloc(0, 998))
		bSuccess = FALSE;

	if (!My_LocalAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_LocalReAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_HeapAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_HeapReAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_VirtualAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_VirtualAllocEx(0, 998))
		bSuccess = FALSE;

	if (!My_GlobalAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_GlobalReAlloc(0, 998))
		bSuccess = FALSE;

	if (!My_CopyMemory(0, 998))
		bSuccess = FALSE;

	if (!My_MoveMemory(0, 998))
		bSuccess = FALSE;


// return true if we make it through all function calls...
return bSuccess;
}


bool SegmentLocked()
{	
	bool bSuccess = TRUE;

	if (!My_LocalFree(0, 212))
		bSuccess = FALSE;

	if (!My_GlobalFree(0, 212))
		bSuccess = FALSE;

	if (!My_VirtualFree(0, 212))
		bSuccess = FALSE;

	if (!My_VirtualFreeEx(0, 212))
		bSuccess = FALSE;

	if (!My_HeapFree(0, 212))
		bSuccess = FALSE;

	if (!My_HeapDestroy(0, 212))
		bSuccess = FALSE;

	if (!My_MoveMemory(0, 212))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool InvalidAddress()
{	
	bool bSuccess = TRUE;

	if (!My_malloc(0, 487))
		bSuccess = FALSE;

	if (!My_LocalAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_LocalReAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_HeapAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_HeapReAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_VirtualAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_VirtualAllocEx(0, 487))
		bSuccess = FALSE;

	if (!My_GlobalAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_GlobalReAlloc(0, 487))
		bSuccess = FALSE;

	if (!My_CopyMemory(0, 487))
		bSuccess = FALSE;

	if (!My_MoveMemory(0, 487))
		bSuccess = FALSE;

	if (!My_LocalFree(0, 487))
		bSuccess = FALSE;

	if (!My_GlobalFree(0, 487))
		bSuccess = FALSE;

	if (!My_VirtualFree(0, 487))
		bSuccess = FALSE;

	if (!My_VirtualFreeEx(0, 487))
		bSuccess = FALSE;

	if (!My_HeapFree(0, 487))
		bSuccess = FALSE;

	if (!My_HeapDestroy(0, 487))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool PagingFileTooSmall()
{	
	bool bSuccess = TRUE;

	if (!My_LocalAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_LocalReAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_HeapCreate(0, 1454))
		bSuccess = FALSE;

	if (!My_HeapAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_HeapReAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_VirtualAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_VirtualAllocEx(0, 1454))
		bSuccess = FALSE;

	if (!My_GlobalAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_GlobalReAlloc(0, 1454))
		bSuccess = FALSE;

	if (!My_CopyMemory(0, 1454))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}
