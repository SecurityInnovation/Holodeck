// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WIN32_WINNT	 0x0500
#define WINVER			 0x0500
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>


// since NULL values make Holodeck crash, we will create a value that is not NULL...
#define STRING_VAL	"C:\\MY FILE"
#define LSTRING_VAL L"C:\\MY FILE"


// Useful Functions...
void promptForStarting();
bool checkErrorCode(DWORD iErrorCode);
bool checkReturnValue(DWORD dwReturnExpected, DWORD dwReturnActual);
void uninterceptedPrint(char * pszString);
void uninterceptedGetChar();
void getInterceptionDisablers();

DWORD CALLBACK GENERIC_PROGRESS_ROUTINE(
  LARGE_INTEGER TotalFileSize,          // file size
  LARGE_INTEGER TotalBytesTransferred,  // bytes transferred
  LARGE_INTEGER StreamSize,             // bytes in stream
  LARGE_INTEGER StreamBytesTransferred, // bytes transferred for stream
  DWORD dwStreamNumber,                 // current stream
  DWORD dwCallbackReason,               // callback reason
  HANDLE hSourceFile,                   // handle to source file
  HANDLE hDestinationFile,              // handle to destination file
  LPVOID lpData                         // from CopyFileEx
);

VOID CALLBACK GENERIC_COMPLETION_ROUTINE (
  DWORD dwErrorCode,                // completion code
  DWORD dwNumberOfBytesTransfered,  // number of bytes transferred
  LPOVERLAPPED lpOverlapped         // I/O information buffer
);

// File Test Cases...
bool FileNotFound();
bool FileCannotBeAccessed();
bool AccessDenied();
bool WriteProtected();
bool CrcDataError();
bool DriveCannotSeekDisk();
bool FileInUse();
bool FileLocked();
bool DiskFull();
bool FileAlreadyExists();
bool FileCannotBeCreated();
bool FileNameTooLong();
bool DirectoryReadOnly();
bool PathNotFound();
bool DirectoryCannotBeRemoved();
bool DirectoryNotEmpty();
bool PathInvalid();
bool CorruptStructure();

// Memory Test Cases...
bool InsufficientMemory();
bool InvalidAccessToLocation();
bool SegmentLocked();
bool InvalidAddress();
bool PagingFileTooSmall();

// Network Test Cases...
bool Disconnected();
bool NetworkNotInstalled();
bool WrongWinsockVersion();
bool WinsockTaskLimitReached();
bool AllPortsUnavailable();
bool NetworkIsDown();

// File Functions...
bool My_CopyFileA(DWORD ret, DWORD error);
bool My_CopyFileW(DWORD ret, DWORD error);
bool My_CopyFileExA(DWORD ret, DWORD error);
bool My_CopyFileExW(DWORD ret, DWORD error);
bool My_CreateFileA(HANDLE ret, DWORD error);
bool My_CreateFileW(HANDLE ret, DWORD error);
bool My_DeleteFileA(DWORD ret, DWORD error);
bool My_DeleteFileW(DWORD ret, DWORD error);
bool My_GetFileAttributesA(DWORD ret, DWORD error);
bool My_GetFileAttributesW(DWORD ret, DWORD error);
bool My_GetFileAttributesExA(DWORD ret, DWORD error);
bool My_GetFileAttributesExW(DWORD ret, DWORD error);
bool My_MoveFileA(DWORD ret, DWORD error);
bool My_MoveFileW(DWORD ret, DWORD error);
bool My_MoveFileExA(DWORD ret, DWORD error);
bool My_MoveFileExW(DWORD ret, DWORD error);
bool My_MoveFileWithProgressA(DWORD ret, DWORD error);
bool My_MoveFileWithProgressW(DWORD ret, DWORD error);
bool My_ReplaceFileA(DWORD ret, DWORD error);
bool My_ReplaceFileW(DWORD ret, DWORD error);
bool My_SetFileAttributesA(DWORD ret, DWORD error);
bool My_SetFileAttributesW(DWORD ret, DWORD error);
bool My_GetBinaryTypeA(DWORD ret, DWORD error);
bool My_GetBinaryTypeW(DWORD ret, DWORD error);
bool My_RemoveDirectoryA(DWORD ret, DWORD error);
bool My_RemoveDirectoryW(DWORD ret, DWORD error);
bool My_LoadLibraryExA(HMODULE ret, DWORD error);
bool My_LoadLibraryExW(HMODULE ret, DWORD error);
bool My_LoadLibraryA(HMODULE ret, DWORD error);
bool My_LoadLibraryW(HMODULE ret, DWORD error);
bool My_LoadLibraryW(HMODULE ret, DWORD error);
bool My_CreateDirectoryA(DWORD ret, DWORD error);
bool My_CreateDirectoryW(DWORD ret, DWORD error);
bool My_CreateDirectoryExA(DWORD ret, DWORD error);
bool My_CreateDirectoryExW(DWORD ret, DWORD error);
bool My_GetDiskFreeSpaceA(DWORD ret, DWORD error);
bool My_GetDiskFreeSpaceW(DWORD ret, DWORD error);
bool My_GetDiskFreeSpaceExA(DWORD ret, DWORD error);
bool My_GetDiskFreeSpaceExW(DWORD ret, DWORD error);
bool My_GetDriveTypeA(DWORD ret, DWORD error);
bool My_GetDriveTypeW(DWORD ret, DWORD error);
bool My_GetLogicalDrives(DWORD ret, DWORD error);
bool My_GetLogicalDriveStringsA(DWORD ret, DWORD error);
bool My_GetLogicalDriveStringsW(DWORD ret, DWORD error);
bool My_GetFileSizeEx(DWORD ret, DWORD error);
bool My_GetFileSize(DWORD ret, DWORD error);
bool My_GetCurrentDirectoryA(DWORD ret, DWORD error);
bool My_GetCurrentDirectoryW(DWORD ret, DWORD error);
bool My_WriteFile(DWORD ret, DWORD error);
bool My_WriteFileEx(DWORD ret, DWORD error);
bool My_WriteFileGather(DWORD ret, DWORD error);
bool My_ReadFile(DWORD ret, DWORD error);
bool My_ReadFileEx(DWORD ret, DWORD error);
bool My_ReadFileScatter(DWORD ret, DWORD error);
bool My_MapViewOfFile(LPVOID ret, DWORD error);
bool My_MapViewOfFileEx(LPVOID ret, DWORD error);
bool My_SearchPathA(DWORD ret, DWORD error);
bool My_SearchPathW(DWORD ret, DWORD error);


// Memory Functions...
bool My_malloc(VOID * ret, DWORD error);
bool My_LocalAlloc(HLOCAL ret, DWORD error);
bool My_LocalReAlloc(HLOCAL ret, DWORD error);
bool My_HeapCreate(HANDLE ret, DWORD error);
bool My_HeapAlloc(LPVOID ret, DWORD error);
bool My_HeapReAlloc(LPVOID ret, DWORD error);
bool My_VirtualAlloc(LPVOID ret, DWORD error);
bool My_VirtualAllocEx(LPVOID ret, DWORD error);
bool My_GlobalAlloc(HGLOBAL ret, DWORD error);
bool My_GlobalReAlloc(HGLOBAL ret, DWORD error);
bool My_CopyMemory(DWORD ret, DWORD error);
bool My_MoveMemory(DWORD ret, DWORD error);
bool My_LocalFree(HLOCAL ret, DWORD error);
bool My_GlobalFree(HGLOBAL ret, DWORD error);
bool My_VirtualFree(BOOL ret, DWORD error);
bool My_VirtualFreeEx(BOOL ret, DWORD error);
bool My_HeapFree(BOOL ret, DWORD error);
bool My_HeapDestroy(BOOL ret, DWORD error);


// Network Functions...
bool My_WSAConnect(DWORD ret, DWORD error);
bool My_connect(DWORD ret, DWORD error);
bool My_WSARecv(DWORD ret, DWORD error);
bool My_recv(DWORD ret, DWORD error);
bool My_WSASend(DWORD ret, DWORD error);
bool My_send(DWORD ret, DWORD error);
bool My_WSAStartup(DWORD ret, DWORD error);
bool My_ioctlsocket(DWORD ret, DWORD error);
bool My_select(DWORD ret, DWORD error);
bool My_socket(DWORD ret, DWORD error);
bool My_WSAEventSelect(DWORD ret, DWORD error);
bool My_WSAIoctl(DWORD ret, DWORD error);
bool My_WSASocketA(DWORD ret, DWORD error);
bool My_WSASocketW(DWORD ret, DWORD error);