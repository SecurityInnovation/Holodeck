#include "stdafx.h"

extern BOOL BOOL_VAL;
extern DWORD INT_VAL;
extern SECURITY_ATTRIBUTES GENERIC_SECURITY;
extern FILE_SEGMENT_ELEMENT GENERIC_SEGMENT_ARRAY[];
extern HANDLE GENERIC_HANDLE;
extern CHAR OUT_STRING_VAL[];
extern WCHAR WOUT_STRING_VAL[];

bool My_CopyFileA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CopyFileA(STRING_VAL, STRING_VAL, BOOL_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CopyFileA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CopyFileA FAILED!\n");
	return FALSE;
}

bool My_CopyFileW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CopyFileW(LSTRING_VAL, LSTRING_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CopyFileW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CopyFileW FAILED!\n");
	return FALSE;
}

bool My_CopyFileExA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CopyFileExA(STRING_VAL, STRING_VAL, GENERIC_PROGRESS_ROUTINE, &INT_VAL, &BOOL_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CopyFileExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CopyFileExA FAILED!\n");
	return FALSE;
}

bool My_CopyFileExW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CopyFileExW(LSTRING_VAL, LSTRING_VAL, GENERIC_PROGRESS_ROUTINE, &INT_VAL, &BOOL_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CopyFileExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CopyFileExW FAILED!\n");
	return FALSE;
}

bool My_CreateFileA(HANDLE ret, DWORD error)
{
	HANDLE dwRet = (HANDLE)-20;

	////
	dwRet = CreateFileA(STRING_VAL, INT_VAL, INT_VAL, &GENERIC_SECURITY, INT_VAL, INT_VAL, GENERIC_HANDLE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateFileA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateFileA FAILED!\n");
	return FALSE;
}

bool My_CreateFileW(HANDLE ret, DWORD error)
{
	HANDLE dwRet = (HANDLE)-20;

	////
	dwRet = CreateFileW(LSTRING_VAL, INT_VAL, INT_VAL, &GENERIC_SECURITY, INT_VAL, INT_VAL, GENERIC_HANDLE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateFileW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateFileW FAILED!\n");
	return FALSE;
}

bool My_DeleteFileA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = DeleteFileA(STRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("DeleteFileA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("DeleteFileA FAILED!\n");
	return FALSE;
}

bool My_DeleteFileW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = DeleteFileW(LSTRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("DeleteFileW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("DeleteFileW FAILED!\n");
	return FALSE;
}

bool My_GetFileAttributesA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileAttributesA(STRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileAttributesA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileAttributesA FAILED!\n");
	return FALSE;
}


bool My_GetFileAttributesW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileAttributesW(LSTRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileAttributesW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileAttributesW FAILED!\n");
	return FALSE;
}


bool My_GetFileAttributesExA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileAttributesExA(STRING_VAL, GetFileExInfoStandard, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileAttributesExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileAttributesExA FAILED!\n");
	return FALSE;
}

bool My_GetFileAttributesExW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileAttributesExW(LSTRING_VAL, GetFileExInfoStandard, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileAttributesExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileAttributesExW FAILED!\n");
	return FALSE;
}

bool My_MoveFileA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileA(STRING_VAL, STRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileA FAILED!\n");
	return FALSE;
}

bool My_MoveFileW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileW(LSTRING_VAL, LSTRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileW FAILED!\n");
	return FALSE;
}

bool My_MoveFileExA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileExA(STRING_VAL, STRING_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileExA FAILED!\n");
	return FALSE;
}

bool My_MoveFileExW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileExW(LSTRING_VAL, LSTRING_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileExW FAILED!\n");
	return FALSE;
}

bool My_MoveFileWithProgressA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileWithProgressA(STRING_VAL, STRING_VAL, GENERIC_PROGRESS_ROUTINE, &INT_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileWithProgressA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileWithProgressA FAILED!\n");
	return FALSE;
}

bool My_MoveFileWithProgressW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = MoveFileWithProgressW(LSTRING_VAL, LSTRING_VAL, GENERIC_PROGRESS_ROUTINE, &INT_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MoveFileWithProgressW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MoveFileWithProgressW FAILED!\n");
	return FALSE;
}

bool My_ReplaceFileA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = ReplaceFileA(STRING_VAL, STRING_VAL, STRING_VAL, INT_VAL, &INT_VAL, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ReplaceFileA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ReplaceFileA FAILED!\n");
	return FALSE;
}

bool My_ReplaceFileW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = ReplaceFileW(LSTRING_VAL, LSTRING_VAL, LSTRING_VAL, INT_VAL, &INT_VAL, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ReplaceFileW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ReplaceFileW FAILED!\n");
	return FALSE;
}

bool My_SetFileAttributesA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = SetFileAttributesA(STRING_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("SetFileAttributesA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("SetFileAttributesA FAILED!\n");
	return FALSE;
}

bool My_SetFileAttributesW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = SetFileAttributesW(LSTRING_VAL, INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("SetFileAttributesW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("SetFileAttributesW FAILED!\n");
	return FALSE;
}

bool My_GetBinaryTypeA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetBinaryTypeA(STRING_VAL, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetBinaryTypeA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetBinaryTypeA FAILED!\n");
	return FALSE;
}

bool My_GetBinaryTypeW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetBinaryTypeW(LSTRING_VAL, &INT_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetBinaryTypeW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetBinaryTypeW FAILED!\n");
	return FALSE;
}

bool My_RemoveDirectoryA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = RemoveDirectoryA(STRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("RemoveDirectoryA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("RemoveDirectoryA FAILED!\n");
	return FALSE;
}

bool My_RemoveDirectoryW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = RemoveDirectoryW(LSTRING_VAL);

	if (!checkReturnValue(ret, dwRet))
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("RemoveDirectoryW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("RemoveDirectoryW FAILED!\n");
	return FALSE;
}

bool My_LoadLibraryExA(HMODULE ret, DWORD error)
{
	HMODULE dwRet = (HMODULE)-20;

	////
	dwRet = LoadLibraryExA(STRING_VAL, GENERIC_HANDLE, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LoadLibraryExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LoadLibraryExA FAILED!\n");
	return FALSE;
}

bool My_LoadLibraryExW(HMODULE ret, DWORD error)
{
	HMODULE dwRet = 0;

	////
	dwRet = LoadLibraryExW(LSTRING_VAL, GENERIC_HANDLE, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LoadLibraryExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LoadLibraryExW FAILED!\n");
	return FALSE;
}

bool My_LoadLibraryA(HMODULE ret, DWORD error)
{
	HMODULE dwRet = (HMODULE)-20;

	////
	dwRet = LoadLibraryA(STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LoadLibraryA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LoadLibraryA FAILED!\n");
	return FALSE;
}

bool My_LoadLibraryW(HMODULE ret, DWORD error)
{
	HMODULE dwRet = (HMODULE)-20;

	////
	dwRet = LoadLibraryW(LSTRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("LoadLibraryW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("LoadLibraryW FAILED!\n");
	return FALSE;
}

bool My_CreateDirectoryA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CreateDirectoryA(STRING_VAL, &GENERIC_SECURITY);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateDirectoryA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateDirectoryA FAILED!\n");
	return FALSE;
}

bool My_CreateDirectoryW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CreateDirectoryW(LSTRING_VAL, &GENERIC_SECURITY);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateDirectoryW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateDirectoryW FAILED!\n");
	return FALSE;
}

bool My_CreateDirectoryExA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CreateDirectoryExA(STRING_VAL, STRING_VAL, &GENERIC_SECURITY);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateDirectoryExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateDirectoryExA FAILED!\n");
	return FALSE;
}

bool My_CreateDirectoryExW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = CreateDirectoryExW(LSTRING_VAL, LSTRING_VAL, &GENERIC_SECURITY);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("CreateDirectoryExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("CreateDirectoryExW FAILED!\n");
	return FALSE;
}

bool My_GetDiskFreeSpaceA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDiskFreeSpaceA(STRING_VAL, &INT_VAL, &INT_VAL, &INT_VAL, &INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDiskFreeSpaceA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDiskFreeSpaceA FAILED!\n");
	return FALSE;
}

bool My_GetDiskFreeSpaceW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDiskFreeSpaceW(LSTRING_VAL, &INT_VAL, &INT_VAL, &INT_VAL, &INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDiskFreeSpaceW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDiskFreeSpaceW FAILED!\n");
	return FALSE;
}

bool My_GetDiskFreeSpaceExA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDiskFreeSpaceExA(STRING_VAL, (PULARGE_INTEGER)&INT_VAL, (PULARGE_INTEGER)&INT_VAL, (PULARGE_INTEGER)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDiskFreeSpaceExA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDiskFreeSpaceExA FAILED!\n");
	return FALSE;
}

bool My_GetDiskFreeSpaceExW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDiskFreeSpaceExW(LSTRING_VAL, (PULARGE_INTEGER)&INT_VAL, (PULARGE_INTEGER)&INT_VAL, (PULARGE_INTEGER)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDiskFreeSpaceExW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDiskFreeSpaceExW FAILED!\n");
	return FALSE;
}

bool My_GetDriveTypeA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDriveTypeA(STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDriveTypeA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDriveTypeA FAILED!\n");
	return FALSE;
}

bool My_GetDriveTypeW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetDriveTypeW(LSTRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetDriveTypeW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetDriveTypeW FAILED!\n");
	return FALSE;
}

bool My_GetLogicalDrives(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetLogicalDrives();

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetLogicalDrives PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetLogicalDrives FAILED!\n");
	return FALSE;
}

bool My_GetLogicalDriveStringsA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetLogicalDriveStringsA(INT_VAL, &STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetLogicalDriveStringsA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetLogicalDriveStringsA FAILED!\n");
	return FALSE;
}

bool My_GetLogicalDriveStringsW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetLogicalDriveStringsW(INT_VAL, &LSTRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetLogicalDriveStringsW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetLogicalDriveStringsW FAILED!\n");
	return FALSE;
}

bool My_GetFileSize(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileSize(&GENERIC_HANDLE, &INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileSize PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileSize FAILED!\n");
	return FALSE;
}

bool My_GetFileSizeEx(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetFileSizeEx(&GENERIC_HANDLE, (PLARGE_INTEGER)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetFileSizeEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetFileSizeEx FAILED!\n");
	return FALSE;
}

bool My_GetCurrentDirectoryA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetCurrentDirectoryA(INT_VAL, OUT_STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetCurrentDirectoryA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetCurrentDirectoryA FAILED!\n");
	return FALSE;
}

bool My_GetCurrentDirectoryW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = GetCurrentDirectoryW(INT_VAL, WOUT_STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("GetCurrentDirectoryW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("GetCurrentDirectoryW FAILED!\n");
	return FALSE;
}

bool My_WriteFile(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = WriteFile(GENERIC_HANDLE, &INT_VAL, INT_VAL, &INT_VAL, (LPOVERLAPPED)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WriteFile PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WriteFile FAILED!\n");
	return FALSE;
}

bool My_WriteFileEx(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = WriteFileEx(GENERIC_HANDLE, &INT_VAL, INT_VAL, (LPOVERLAPPED)&INT_VAL, GENERIC_COMPLETION_ROUTINE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WriteFileEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WriteFileEx FAILED!\n");
	return FALSE;
}

bool My_WriteFileGather(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = WriteFileGather(GENERIC_HANDLE, GENERIC_SEGMENT_ARRAY, INT_VAL, &INT_VAL, (LPOVERLAPPED)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WriteFileGather PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WriteFileGather FAILED!\n");
	return FALSE;
}

bool My_ReadFile(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = ReadFile(GENERIC_HANDLE, &INT_VAL, INT_VAL, &INT_VAL, (LPOVERLAPPED)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ReadFile PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ReadFile FAILED!\n");
	return FALSE;
}

bool My_ReadFileEx(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = ReadFileEx(GENERIC_HANDLE, &INT_VAL, INT_VAL, (LPOVERLAPPED)&INT_VAL, GENERIC_COMPLETION_ROUTINE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ReadFileEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ReadFileEx FAILED!\n");
	return FALSE;
}

bool My_ReadFileScatter(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = ReadFileScatter(GENERIC_HANDLE, GENERIC_SEGMENT_ARRAY, INT_VAL, &INT_VAL, (LPOVERLAPPED)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ReadFileScatter PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ReadFileScatter FAILED!\n");
	return FALSE;
}

bool My_MapViewOfFile(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	////
	dwRet = MapViewOfFile(GENERIC_HANDLE, INT_VAL, INT_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MapViewOfFile PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MapViewOfFile FAILED!\n");
	return FALSE;
}

bool My_MapViewOfFileEx(LPVOID ret, DWORD error)
{
	LPVOID dwRet = 0;

	////
	dwRet = MapViewOfFileEx(GENERIC_HANDLE, INT_VAL, INT_VAL, INT_VAL, INT_VAL, (LPVOID)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("MapViewOfFileEx PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("MapViewOfFileEx FAILED!\n");
	return FALSE;
}

bool My_SearchPathA(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = SearchPathA(STRING_VAL, STRING_VAL, STRING_VAL, INT_VAL, STRING_VAL, NULL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("SearchPathA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("SearchPathA FAILED!\n");
	return FALSE;
}

bool My_SearchPathW(DWORD ret, DWORD error)
{
	DWORD dwRet = 20;

	////
	dwRet = SearchPathW(LSTRING_VAL, LSTRING_VAL, LSTRING_VAL, INT_VAL, LSTRING_VAL, NULL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("SearchPathW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("SearchPathW FAILED!\n");
	return FALSE;
}