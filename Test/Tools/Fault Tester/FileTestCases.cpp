#include "stdafx.h"

extern FARPROC disableInterception;
extern FARPROC enableInterception;

bool FileNotFound()
{	
	bool bSuccess = TRUE;

	if (disableInterception != NULL)
			disableInterception();

	system("echo \"hello\" > \"C:\\My File\"");

	if (enableInterception != NULL)
			enableInterception();

	if (!My_CopyFileA(0, 2))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 2))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 2))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 2))
		bSuccess = FALSE;

	if (!My_CreateFileA(INVALID_HANDLE_VALUE, 2))
		bSuccess = FALSE;

	if (!My_CreateFileW(INVALID_HANDLE_VALUE, 2))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(-1, 2))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(-1, 2))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 2))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 2))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 2))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 2))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 2))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 2))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 2))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 2))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 2))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 2))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 2))
		bSuccess = FALSE;

	if (!My_LoadLibraryExA(0, 2))
		bSuccess = FALSE;

	if (!My_LoadLibraryExW(0, 2))
		bSuccess = FALSE;

	if (!My_LoadLibraryA(0, 2))
		bSuccess = FALSE;

	if (!My_LoadLibraryW(0, 2))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 2))
		bSuccess = FALSE;

	if (disableInterception != NULL)
			disableInterception();

	system("echo \"hello\" > \"C:\\My File\"");

	if (enableInterception != NULL)
			enableInterception();

	if (!My_DeleteFileW(0, 2))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool FileCannotBeAccessed()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 4))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 4))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 4))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 4))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 4))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 4))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 4))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 4))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 4))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 4))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 4))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 4))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 4))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 4))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 4))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 4))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 4))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 4))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 4))
		bSuccess = FALSE;

	if (!My_LoadLibraryExA(0, 4))
		bSuccess = FALSE;

	if (!My_LoadLibraryExW(0, 4))
		bSuccess = FALSE;

	if (!My_LoadLibraryA(0, 4))
		bSuccess = FALSE;

	if (!My_LoadLibraryW(0, 4))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool AccessDenied()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 5))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 5))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 5))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 5))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 5))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 5))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 5))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 5))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 5))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 5))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 5))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 5))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 5))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 5))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 5))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 5))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 5))
		bSuccess = FALSE;

	if (!My_GetCurrentDirectoryA(0, 5))
		bSuccess = FALSE;

	if (!My_GetCurrentDirectoryW(0, 5))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceA(0, 5))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceW(0, 5))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExA(0, 5))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExW(0, 5))
		bSuccess = FALSE;

	if (!My_GetDriveTypeA(0, 5))
		bSuccess = FALSE;

	if (!My_GetDriveTypeW(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileSize(0, 5))
		bSuccess = FALSE;

	if (!My_GetFileSizeEx(0, 5))
		bSuccess = FALSE;

	if (!My_GetLogicalDrives(0, 5))
		bSuccess = FALSE;

	if (!My_GetLogicalDriveStringsA(0, 5))
		bSuccess = FALSE;

	if (!My_GetLogicalDriveStringsW(0, 5))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 5))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 5))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 5))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 5))
		bSuccess = FALSE;

	if (!My_LoadLibraryExA(0, 5))
		bSuccess = FALSE;

	if (!My_LoadLibraryExW(0, 5))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool WriteProtected()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 19))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 19))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 19))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 19))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 19))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 19))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 19))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 19))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 19))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 19))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 19))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 19))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 19))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 19))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 19))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 19))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 19))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 19))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 19))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 19))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 19))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 19))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 19))
		bSuccess = FALSE;

	if (!My_WriteFile(0, 19))
		bSuccess = FALSE;

	if (!My_WriteFileEx(0, 19))
		bSuccess = FALSE;

	if (!My_WriteFileGather(0, 19))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool CrcDataError()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 23))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 23))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 23))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 23))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 23))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 23))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 23))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 23))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 23))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 23))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 23))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 23))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 23))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 23))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 23))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 23))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 23))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 23))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 23))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 23))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 23))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 23))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 23))
		bSuccess = FALSE;

	if (!My_WriteFile(0, 23))
		bSuccess = FALSE;

	if (!My_WriteFileEx(0, 23))
		bSuccess = FALSE;

	if (!My_WriteFileGather(0, 23))
		bSuccess = FALSE;

	if (!My_MapViewOfFile(0, 23))
		bSuccess = FALSE;

	if (!My_MapViewOfFileEx(0, 23))
		bSuccess = FALSE;

	if (!My_ReadFile(0, 23))
		bSuccess = FALSE;
	
	if (!My_ReadFileEx(0, 23))
		bSuccess = FALSE;

	if (!My_ReadFileScatter(0, 23))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 23))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 23))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool DriveCannotSeekDisk()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 25))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 25))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 25))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 25))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 25))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 25))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 25))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 25))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 25))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 25))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 25))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 25))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 25))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 25))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 25))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 25))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 25))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 25))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 25))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 25))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 25))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 25))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 25))
		bSuccess = FALSE;

	if (!My_WriteFile(0, 25))
		bSuccess = FALSE;

	if (!My_WriteFileEx(0, 25))
		bSuccess = FALSE;

	if (!My_WriteFileGather(0, 25))
		bSuccess = FALSE;

	if (!My_MapViewOfFile(0, 25))
		bSuccess = FALSE;

	if (!My_MapViewOfFileEx(0, 25))
		bSuccess = FALSE;

	if (!My_ReadFile(0, 25))
		bSuccess = FALSE;
	
	if (!My_ReadFileEx(0, 25))
		bSuccess = FALSE;

	if (!My_ReadFileScatter(0, 25))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 25))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 25))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool FileInUse()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 32))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 32))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 32))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 32))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 32))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 32))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 32))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 32))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 32))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 32))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 32))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 32))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 32))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 32))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 32))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool FileLocked()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 32))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 32))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 32))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 32))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 32))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 32))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 32))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 32))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 32))
		bSuccess = FALSE;

	if (!My_WriteFile(0, 32))
		bSuccess = FALSE;

	if (!My_WriteFileEx(0, 32))
		bSuccess = FALSE;

	if (!My_WriteFileGather(0, 32))
		bSuccess = FALSE;

	if (!My_MapViewOfFile(0, 32))
		bSuccess = FALSE;

	if (!My_MapViewOfFileEx(0, 32))
		bSuccess = FALSE;

	if (!My_ReadFile(0, 32))
		bSuccess = FALSE;
	
	if (!My_ReadFileEx(0, 32))
		bSuccess = FALSE;

	if (!My_ReadFileScatter(0, 32))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool DiskFull()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 112))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 112))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 112))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 112))
		bSuccess = FALSE;

	if (!My_WriteFile(0, 112))
		bSuccess = FALSE;

	if (!My_WriteFileEx(0, 112))
		bSuccess = FALSE;

	if (!My_WriteFileGather(0, 112))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool FileAlreadyExists()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 80))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 80))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 80))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 80))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 80))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 80))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 80))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 80))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 80))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 80))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 80))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool FileCannotBeCreated()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 82))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool FileNameTooLong()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 111))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 111))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 111))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 111))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 111))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 111))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 111))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 111))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 111))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 111))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 111))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 111))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 111))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 111))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 111))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 111))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 111))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 111))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 111))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 111))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 111))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 111))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 111))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 111))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 111))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceA(0, 111))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceW(0, 111))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExA(0, 111))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExW(0, 111))
		bSuccess = FALSE;

	if (!My_GetDriveTypeA(0, 111))
		bSuccess = FALSE;

	if (!My_GetDriveTypeW(0, 111))
		bSuccess = FALSE;

	if (!My_SearchPathA(0, 111))
		bSuccess = FALSE;

	if (!My_SearchPathW(0, 111))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool DirectoryReadOnly()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 6009))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 6009))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 6009))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 6009))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 6009))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 6009))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 6009))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 6009))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 6009))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 6009))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 6009))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 6009))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 6009))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 6009))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 6009))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 6009))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 6009))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 6009))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 6009))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool PathNotFound()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 3))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 3))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 3))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 3))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 3))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 3))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 3))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 3))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 3))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 3))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 3))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 3))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 3))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 3))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 3))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 3))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 3))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 3))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 3))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceA(0, 3))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceW(0, 3))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExA(0, 3))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExW(0, 3))
		bSuccess = FALSE;

	if (!My_GetDriveTypeA(0, 3))
		bSuccess = FALSE;

	if (!My_GetDriveTypeW(0, 3))
		bSuccess = FALSE;

	if (!My_SearchPathA(0, 3))
		bSuccess = FALSE;

	if (!My_SearchPathW(0, 3))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool DirectoryCannotBeRemoved()
{	
	bool bSuccess = TRUE;

	if (!My_RemoveDirectoryA(0, 16))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 16))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}


bool DirectoryNotEmpty()
{	
	bool bSuccess = TRUE;

	if (!My_RemoveDirectoryA(0, 145))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 145))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool PathInvalid()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 82))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 82))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 82))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateFileA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateFileW(0, 82))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 82))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 82))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 82))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 82))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 82))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 82))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 82))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 82))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 82))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 82))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 82))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 82))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 82))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 82))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceA(0, 82))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceW(0, 82))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExA(0, 82))
		bSuccess = FALSE;

	if (!My_GetDiskFreeSpaceExW(0, 82))
		bSuccess = FALSE;

	if (!My_GetDriveTypeA(0, 82))
		bSuccess = FALSE;

	if (!My_GetDriveTypeW(0, 82))
		bSuccess = FALSE;

	if (!My_SearchPathA(0, 82))
		bSuccess = FALSE;

	if (!My_SearchPathW(0, 82))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}



bool CorruptStructure()
{	
	bool bSuccess = TRUE;

	if (!My_CopyFileA(0, 1392))
		bSuccess = FALSE;

	if (!My_CopyFileW(0, 1392))
		bSuccess = FALSE;

	if (!My_CopyFileExA(0, 1392))
		bSuccess = FALSE;

	if (!My_CopyFileExW(0, 1392))
		bSuccess = FALSE;

	if (!My_DeleteFileA(0, 1392))
		bSuccess = FALSE;

	if (!My_DeleteFileW(0, 1392))
		bSuccess = FALSE;

	if (!My_GetFileAttributesA(0, 1392))
		bSuccess = FALSE;

	if (!My_GetFileAttributesW(0, 1392))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExA(0, 1392))
		bSuccess = FALSE;

	if (!My_GetFileAttributesExW(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileA(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileW(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileExA(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileExW(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressA(0, 1392))
		bSuccess = FALSE;

	if (!My_MoveFileWithProgressW(0, 1392))
		bSuccess = FALSE;

	if (!My_ReplaceFileA(0, 1392))
		bSuccess = FALSE;

	if (!My_ReplaceFileW(0, 1392))
		bSuccess = FALSE;

	if (!My_SetFileAttributesA(0, 1392))
		bSuccess = FALSE;

	if (!My_SetFileAttributesW(0, 1392))
		bSuccess = FALSE;

	if (!My_CreateDirectoryA(0, 1392))
		bSuccess = FALSE;

	if (!My_CreateDirectoryW(0, 1392))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExA(0, 1392))
		bSuccess = FALSE;

	if (!My_CreateDirectoryExW(0, 1392))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryA(0, 1392))
		bSuccess = FALSE;

	if (!My_RemoveDirectoryW(0, 1392))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeA(0, 1392))
		bSuccess = FALSE;

	if (!My_GetBinaryTypeW(0, 1392))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}
