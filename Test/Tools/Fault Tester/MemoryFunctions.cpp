#include "stdafx.h"

extern BOOL BOOL_VAL;
extern DWORD INT_VAL;
extern SECURITY_ATTRIBUTES GENERIC_SECURITY;
extern FILE_SEGMENT_ELEMENT GENERIC_SEGMENT_ARRAY[];
extern HANDLE GENERIC_HANDLE;
extern FARPROC disableInterception;
extern FARPROC enableInterception;

bool My_malloc(VOID * ret, DWORD error)
{
	VOID * dwRet = 0;

	////
	dwRet = malloc(INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("malloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("malloc FAILED!\n");
	return FALSE;
}

bool My_LocalAlloc(HLOCAL ret, DWORD error)
{
	HLOCAL dwRet = 0;

	////
	dwRet = LocalAlloc(LMEM_FIXED, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LocalAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LocalAlloc FAILED!\n");
	return FALSE;
}

bool My_LocalReAlloc(HLOCAL ret, DWORD error)
{
	HLOCAL dwRet = 0;

	////
	dwRet = LocalReAlloc(GENERIC_HANDLE, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LocalReAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LocalReAlloc FAILED!\n");
	return FALSE;
}

bool My_HeapCreate(HANDLE ret, DWORD error)
{
	HANDLE dwRet = 0;

	////
	dwRet = HeapCreate(INT_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("HeapCreate PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("HeapCreate FAILED!\n");
	return FALSE;
}

bool My_HeapAlloc(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	////
	dwRet = HeapAlloc(GetProcessHeap(), INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("HeapAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("HeapAlloc FAILED!\n");
	return FALSE;
}

bool My_HeapReAlloc(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	CHAR * local_pointer = NULL;

	////
	dwRet = HeapReAlloc(GetProcessHeap(), INT_VAL, local_pointer, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("HeapReAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("HeapReAlloc FAILED!\n");
	return FALSE;
}

bool My_VirtualAlloc(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	////
	dwRet = VirtualAlloc(&STRING_VAL, INT_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("VirtualAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("VirtualAlloc FAILED!\n");
	return FALSE;
}

bool My_VirtualAllocEx(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	////
	dwRet = VirtualAllocEx(GetCurrentProcess(), &STRING_VAL, INT_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("VirtualAllocEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("VirtualAllocEx FAILED!\n");
	return FALSE;
}

bool My_GlobalAlloc(HGLOBAL ret, DWORD error)
{
	HGLOBAL dwRet = 0;

	////
	dwRet = GlobalAlloc(GMEM_FIXED, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GlobalAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GlobalAlloc FAILED!\n");
	return FALSE;
}

bool My_GlobalReAlloc(HGLOBAL ret, DWORD error)
{
	HGLOBAL dwRet = 0;

	////
	dwRet = GlobalReAlloc(GlobalAlloc(GMEM_MOVEABLE, 2), 2, GMEM_MODIFY);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GlobalReAlloc PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GlobalReAlloc FAILED!\n");
	return FALSE;
}

bool My_CopyMemory(DWORD ret, DWORD error)
{
	DWORD dwRet = 0;

	if (disableInterception != NULL)
		disableInterception();

	char * local_pointer = NULL;
	local_pointer = new char[sizeof(STRING_VAL)+1];

	if (enableInterception != NULL)
		enableInterception();

	////
	CopyMemory(local_pointer, &STRING_VAL, sizeof(STRING_VAL));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CopyMemory PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CopyMemory FAILED!\n");
	return FALSE;
}

bool My_MoveMemory(DWORD ret, DWORD error)
{
	DWORD dwRet = 0;

	if (disableInterception != NULL)
		disableInterception();

	char * local_pointer = NULL;
	local_pointer = new char[sizeof(STRING_VAL)+1];

	if (enableInterception != NULL)
		enableInterception();

	////
	MoveMemory(local_pointer, &STRING_VAL, sizeof(STRING_VAL));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveMemory PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveMemory FAILED!\n");
	return FALSE;
}

bool My_LocalFree(HLOCAL ret, DWORD error)
{
	HLOCAL dwRet = 0;

	////
	dwRet = LocalFree(LocalAlloc(LMEM_MOVEABLE, 2));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LocalFree PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LocalFree FAILED!\n");
	return FALSE;
}

bool My_GlobalFree(HGLOBAL ret, DWORD error)
{
	HGLOBAL dwRet = 0;

	////
	dwRet = GlobalFree(GlobalAlloc(GMEM_MOVEABLE, 2));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GlobalFree PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GlobalFree FAILED!\n");
	return FALSE;
}

bool My_VirtualFree(BOOL ret, DWORD error)
{
	BOOL dwRet = 0;

	char * local_pointer = NULL;

	////
	dwRet = VirtualFree(VirtualAlloc(local_pointer, sizeof(char), MEM_COMMIT, PAGE_EXECUTE_READWRITE), sizeof(char), MEM_DECOMMIT);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("VirtualFree PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("VirtualFree FAILED!\n");
	return FALSE;
}

bool My_VirtualFreeEx(BOOL ret, DWORD error)
{
	BOOL dwRet = 0;

	char * local_pointer = NULL;

	////
	dwRet = VirtualFreeEx(GetCurrentProcess(), VirtualAlloc(local_pointer, sizeof(char), MEM_COMMIT, PAGE_EXECUTE_READWRITE), sizeof(char), MEM_DECOMMIT);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("VirtualFreeEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("VirtualFreeEx FAILED!\n");
	return FALSE;
}

bool My_HeapFree(BOOL ret, DWORD error)
{
	BOOL dwRet = 0;

	////
	dwRet = HeapFree(GetProcessHeap(), NULL, HeapAlloc(GetProcessHeap(), NULL, sizeof(char)));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("HeapFree PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("HeapFree FAILED!\n");
	return FALSE;
}

bool My_HeapDestroy(BOOL ret, DWORD error)
{
	BOOL dwRet = 0;

	////
	dwRet = HeapDestroy(HeapCreate(HEAP_NO_SERIALIZE, 0, 0));

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("HeapDestroy PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("HeapDestroy FAILED!\n");
	return FALSE;
}
