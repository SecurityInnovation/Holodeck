// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WIN32_WINNT 0x0501
#define WINVER 0x500 

#include <winsock2.h>
#include <winsafer.h>


//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <lmaccess.h>
#include <Tlhelp32.h>
#include <Lzexpand.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

//typedef LONG NTSTATUS;
// TODO: reference additional headers your program requires here

// FUNCTIONS WITH NO LIB FILE...
typedef NTSYSAPI NTSTATUS (NTAPI *RtlLocalTimeToSystemTime_DEF)(PLARGE_INTEGER, PLARGE_INTEGER);
typedef NTSYSAPI NTSTATUS (NTAPI *NtQuerySystemTime_DEF)(PLARGE_INTEGER);
typedef NTSYSAPI BOOLEAN (*RtlTimeToSecondsSince1970_DEF)(PLARGE_INTEGER, PULONG);

// utility functions...
void enableInterception( );
void disableInterception( );

// Functions...
BOOL My_HeapUnlock( );
BOOL My_AllocateUserPhysicalPages( );
BOOL My_bind ( );
BOOL My_connect ( );
BOOL My_CopyFileA ( );
BOOL My_CopyFileW ( );
BOOL My_CopyFileExA ( );
BOOL My_CopyFileExW ( );
BOOL My_CreateDirectoryA ( );
BOOL My_CreateDirectoryW ( );
BOOL My_CreateDirectoryExA ( );
BOOL My_CreateDirectoryExW ( );
BOOL My_CreateFileA ( );
BOOL My_CreateFileW ( );
BOOL My_CreateProcessA ( );
BOOL My_CreateProcessW ( );
BOOL My_DeleteFileA ( );
BOOL My_DeleteFileW ( );
BOOL My_FlushFileBuffers ( );
BOOL My_FreeLibrary ( );
BOOL My_FreeLibraryAndExitThread ( );
BOOL My_FreeUserPhysicalPages ( );
BOOL My_GetBinaryTypeA ( );
BOOL My_GetBinaryTypeW ( );
BOOL My_GetCurrentDirectoryA ( );
BOOL My_GetCurrentDirectoryW ( );
BOOL My_GetDiskFreeSpaceA ( );
BOOL My_GetDiskFreeSpaceW ( );
BOOL My_GetDiskFreeSpaceExA ( );
BOOL My_GetDiskFreeSpaceExW ( );
BOOL My_GetDriveTypeA ( );
BOOL My_GetDriveTypeW ( );
BOOL My_GetFileAttributesA ( );
BOOL My_GetFileAttributesW ( );
BOOL My_GetFileAttributesExA ( );
BOOL My_GetFileAttributesExW ( );
BOOL My_GetFileInformationByHandle ( );
BOOL My_GetFileSize ( );
BOOL My_GetFileSizeEx ( );
BOOL My_GetLogicalDriveStringsA( );
BOOL My_GetLogicalDriveStringsW( );
BOOL My_GlobalAlloc( );
BOOL My_GlobalFree( );
BOOL My_GlobalMemoryStatus( );
BOOL My_GlobalMemoryStatusEx( );
BOOL My_HeapAlloc( );
BOOL My_HeapCompact( );
BOOL My_HeapCreate( );
BOOL My_HeapDestroy ( );
BOOL My_HeapFree ( );
BOOL My_HeapLock ( );
BOOL My_HeapReAlloc ( );
BOOL My_HeapSize ( );

// Vaibhav's Functions...
BOOL My_HeapUnlock( );
BOOL My_HeapValidate();
BOOL My_ioctlsocket(); 
BOOL My_LoadLibraryA();
BOOL My_LoadLibraryExA();
BOOL My_LoadLibraryExW();
BOOL My_LoadLibraryW();
BOOL My_LocalAlloc();
BOOL My_LocalFree();
BOOL My_LocalReAlloc();
BOOL My_MapUserPhysicalPages();
BOOL My_MapUserPhysicalPagesScatter();
BOOL My_MapViewOfFile();
BOOL My_MapViewOfFileEx();
BOOL My_MoveFileA();
BOOL My_MoveFileExA();
BOOL My_MoveFileExW();
BOOL My_MoveFileW();
BOOL My_MoveFileWithProgressA();
BOOL My_MoveFileWithProgressW();
BOOL My_ReadFile();
BOOL My_ReadFileEx();
BOOL My_ReadFileScatter();
BOOL My_recv();  
BOOL My_RegCloseKey();
BOOL My_RegConnectRegistryA();
BOOL My_RegConnectRegistryW();
BOOL My_RegCreateKeyA(); //problem in holodeck can be bug if problem with HKEY_LOCAL_MACHINE is used
BOOL My_RegCreateKeyExA();//?? problem with HKEY_LOCAL_MACHINE
BOOL My_RegCreateKeyExW();//?? problem with HKEY_LOCAL_MACHINE
BOOL My_RegCreateKeyW();
BOOL My_RegDeleteKeyA();
BOOL My_RegDeleteKeyW();
BOOL My_RegDeleteValueA();//problem in holodeck can be bug
BOOL My_RegDeleteValueW();//problem in holodeck can be bug
BOOL My_RegDisablePredefinedCache();
BOOL My_RegEnumKeyA();
BOOL My_RegEnumKeyExA();
BOOL My_RegEnumKeyExW();
BOOL My_RegEnumKeyW();
BOOL My_RegEnumValueA();
BOOL My_RegEnumValueW();
BOOL My_RegFlushKey();
BOOL My_RegGetKeySecurity();
BOOL My_RegLoadKeyA();
BOOL My_RegLoadKeyW();
BOOL My_RegNotifyChangeKeyValue();
BOOL My_RegOpenCurrentUser();
BOOL My_RegOpenKeyA();
BOOL My_RegOpenKeyExA();
BOOL My_RegOpenKeyExW();
BOOL My_RegOpenKeyW();
BOOL My_RegOpenUserClassesRoot();

// Jessica's Functions...
BOOL My_RegOverridePredefKey();
BOOL My_RegQueryInfoKeyA();
BOOL My_RegQueryInfoKeyW();
BOOL My_RegQueryMultipleValuesA();
BOOL My_RegQueryMultipleValuesW();
BOOL My_RegQueryValueA();
BOOL My_RegQueryValueExA();
BOOL My_RegQueryValueExW();
BOOL My_RegQueryValueW();
BOOL My_RegReplaceKeyA();
BOOL My_RegReplaceKeyW();
BOOL My_RegRestoreKeyA();
BOOL My_RegRestoreKeyW();
BOOL My_RegSaveKeyA();
BOOL My_RegSaveKeyExA();
BOOL My_RegSaveKeyExW();
BOOL My_RegSaveKeyW();
BOOL My_RegSetKeySecurity();
BOOL My_RegSetValueA();
BOOL My_RegSetValueExA();
BOOL My_RegSetValueExW();
BOOL My_RegSetValueW();
BOOL My_RegUnLoadKeyA();
BOOL My_RegUnLoadKeyW();
BOOL My_RemoveDirectoryA();
BOOL My_RemoveDirectoryW();
BOOL My_ReplaceFileA();
BOOL My_ReplaceFileW();
BOOL My_SearchPathA();
BOOL My_SearchPathW();
BOOL My_select();
BOOL My_send();
BOOL My_SetFileAttributesA();
BOOL My_SetFileAttributesW();
BOOL My_socket();
BOOL My_VirtualAlloc();
BOOL My_VirtualAllocEx();
BOOL My_VirtualFree();
BOOL My_VirtualFreeEx();
BOOL My_VirtualLock();
BOOL My_VirtualQuery();
BOOL My_VirtualQueryEx();
BOOL My_VirtualUnlock();
BOOL My_WriteFile();
BOOL My_WriteFileEx();
BOOL My_WriteFileGather();
BOOL My_WSAConnect();
BOOL My_WSAEventSelect();
BOOL My_WSAIoctl();
BOOL My_WSARecv();
BOOL My_WSASend();
BOOL My_WSASocketA();
BOOL My_WSASocketW();
BOOL My_WSAStartup();
BOOL My_SetFilePointerEx ( );
BOOL My_SetFileShortNameA ( );
BOOL My_SetFileShortNameW ( );
BOOL My_SetFileTime ( );
BOOL My_SetFileValidData ( );
BOOL My_SetEndOfFile( );
BOOL My_SetLastError( );
BOOL My_SetPriorityClass( );
BOOL My_SetProcessShutdownParameters( );
BOOL My_SetProcessWorkingSetSize( );
BOOL My_TerminateProcess( );
BOOL My_TerminateThread( );
BOOL My_TlsAlloc( );
BOOL My_TlsFree( );
BOOL My_TlsGetValue( );
BOOL My_TlsSetValue( );
BOOL My_UnlockFile( );
BOOL My_UnlockFileEx( );
BOOL My_VirtualProtect( );
BOOL My_VirtualProtectEx( );
BOOL My_WriteProcessMemory( );
BOOL My__hread( );
BOOL My__hwrite( );
BOOL My__lclose( );
BOOL My__lcreat( );
BOOL My__llseek( );
BOOL My__lopen( );
BOOL My__lread( );
BOOL My__lwrite( );
BOOL My_BackupEventLogA( );
BOOL My_BackupEventLogW( );
BOOL My_ClearEventLogA( );
BOOL My_ClearEventLogW( );
BOOL My_CloseEventLog( );
BOOL My_CreateProcessAsUserA( );
BOOL My_CreateProcessAsUserW( );
BOOL My_CreateProcessWithLogonW( );
BOOL My_DecryptFileA( );
BOOL My_DecryptFileW( );
BOOL My_EncryptFileA( );
BOOL My_EncryptFileW( );
BOOL My_FileEncryptionStatusA( );
BOOL My_FileEncryptionStatusW( );
BOOL My_GetFileSecurityA( );
BOOL My_GetFileSecurityW( );
BOOL My_OpenBackupEventLogA( );
BOOL My_OpenBackupEventLogW( );
BOOL My_OpenEventLogA( );
BOOL My_OpenEventLogW( );
BOOL My_QueryUsersOnEncryptedFile( );
BOOL My_RemoveUsersFromEncryptedFile( );
BOOL My_SaferRecordEventLogEntry( );
BOOL My_SetFileSecurityA( );
BOOL My_SetFileSecurityW( );
BOOL My_WSAAccept( );
BOOL My_WSAAsyncGetHostByAddr( );
BOOL My_WSAAsyncGetHostByName( );
BOOL My_WSAAsyncGetServByName( );
BOOL My_WSAAsyncGetServByPort( );
BOOL My_WSAAsyncSelect( );
BOOL My_WSACancelAsyncRequest( );
BOOL My_WSACleanup( );
BOOL My_WSAProviderConfigChange( );
BOOL My_WSADuplicateSocketA( );
BOOL My_WSADuplicateSocketW( );
BOOL My_WSAGetLastError( );
BOOL My_WSAJoinLeaf( );
BOOL My_WSARecvDisconnect( );
BOOL My_WSARecvFrom( );
BOOL My_WSASendDisconnect( );
BOOL My_WSASendTo( );
BOOL My_WSASetLastError( );
BOOL My_accept( );
BOOL My_gethostbyaddr( );
BOOL My_gethostbyname( );
BOOL My_getsockopt( );
BOOL My_listen( );
BOOL My_recvfrom( );
BOOL My_sendto( );
BOOL My_setsockopt( );
BOOL My_shutdown( );
//BOOL My_SaferiIsExecutableFileType( );
BOOL My_LocalFileTimeToFileTime( );
BOOL My_NtQuerySystemTime( );
BOOL My_QueryPerformanceCounter( );
BOOL My_QueryPerformanceFrequency( );
BOOL My_RtlLocalTimeToSystemTime( );
BOOL My_RtlTimeToSecondsSince1970( );
BOOL My_SetLocalTime( );
BOOL My_SetSystemTime( );
BOOL My_SetSystemTimeAdjustment( );
BOOL My_SetTimeZoneInformation( );
BOOL My_SystemTimeToFileTime( );
BOOL My_SystemTimeToTzSpecificLocalTime( );
BOOL My_TzSpecificLocalTimeToSystemTime( );

BOOL My_AllocConsole();
BOOL My_AreFileApisANSI();
BOOL My_AttachConsole();
BOOL My_BackupRead();
BOOL My_BackupSeek();
BOOL My_BackupWrite();
BOOL My_CancelIo();
BOOL My_CloseHandle();
BOOL My_CopyLZFile(); //??
BOOL My_CreateFileMappingA();
BOOL My_CreateFileMappingW();
BOOL My_CreateHardLinkA();
BOOL My_CreateHardLinkW();
BOOL My_CreateMemoryResourceNotification();//??
BOOL My_CreateThread();
BOOL My_DebugActiveProcess();
BOOL My_ContinueDebugEvent();
BOOL My_DebugActiveProcessStop();
BOOL My_DebugBreakProcess();
BOOL My_DebugSetProcessKillOnExit();
BOOL My_DisableThreadLibraryCalls();
BOOL My_DnsHostnameToComputerNameA();
BOOL My_DnsHostnameToComputerNameW();
BOOL My_FindFirstFileA();
BOOL My_FindFirstFileW();
BOOL My_FindFirstFileExA();
BOOL My_FindFirstFileExW();
BOOL My_FindNextFileA();
BOOL My_FindNextFileW();
BOOL My_GetBinaryType();
BOOL My_GetCompressedFileSizeA();
BOOL My_GetCompressedFileSizeW();
BOOL My_GetExitCodeProcess();
BOOL My_GetExitCodeThread();
BOOL My_GetFullPathNameA();
BOOL My_GetFullPathNameW();
BOOL My_GetProcessHeap();
BOOL My_GetProcessHeaps();
BOOL My_GetProcessIoCounters();
BOOL My_GetProcessTimes();
BOOL My_GetProcessWorkingSetSize();
BOOL My_GetStartupInfoA();
BOOL My_GetStartupInfoW();
BOOL My_GetSystemDirectoryA();
BOOL My_GetSystemDirectoryW();
BOOL My_GetSystemWindowsDirectoryA();
BOOL My_GetSystemWindowsDirectoryW();
BOOL My_GetWriteWatch();
BOOL My_GlobalMemoryStatus();
BOOL My_GlobalReAlloc();
BOOL My_LockFile();
BOOL My_LockFileEx();
BOOL My_LZClose();
BOOL My_LZCopy();
BOOL My_LZDone();
BOOL My_LZRead();
BOOL My_LZSeek();
BOOL My_LoadModule();
BOOL My_OpenFile();
BOOL My_QueryMemoryResourceNotification();
BOOL My_ReplaceFile();
BOOL My_RtlFillMemory();
BOOL My_RtlMoveMemory();
BOOL My_RtlZeroMemory();
BOOL My_SetFileApisToANSI();
BOOL My_SetFileApisToOEM();
BOOL My_SetFilePointer();
BOOL My_CompareFileTime();
BOOL My_DosDateTimeToFileTime();
BOOL My_FileTimeToDosDateTime();
BOOL My_FileTimeToLocalFileTime();
BOOL My_FileTimeToSystemTime();
BOOL My_GetFileTime();
BOOL My_GetLocalTime();
BOOL My_GetSystemTime();
BOOL My_GetSystemTimeAsFileTime();
BOOL My_GetSystemTimes();//??
BOOL My_GetTickCount();
BOOL My_GetTimeFormat();
BOOL My_GetTimeZoneInformation();

BOOL My_CreateFiber();//??

BOOL My_CreateFiberEx();//??

BOOL My_LZOpenFileA();//??

BOOL My_LZOpenFileW();//??

BOOL My_GetSystemTimeAdjustment();//??
//BOOL My_ExitProcess();// ??
//BOOL My_ExitThread();  //??
//BOOL My_DebugBreak(); //??
BOOL My_CreateToolhelp32Snapshot();//??
BOOL My_FatalAppExitA(); //??
//BOOL My_FatalAppExitW();//??
//BOOL My_FatalExit(); //??
BOOL My_DeleteFiber(); //??
//BOOL My_GetSystemTimes( );
BOOL My_CreateMemoryResourceNotification( );
BOOL My_CopyLZFile( );

BOOL My_GetFileType();
BOOL My_GetLogicalDrives();

//Functions added after Release 2.0

BOOL My_GetProcAddress();
BOOL My_GetModuleHandleA(); 
BOOL My_GetModuleHandleW(); 
BOOL My_DeviceIoControl();
BOOL My_GetVersionExA(); 
BOOL My_GetVersionExW(); 
BOOL My_GetSystemInfo(); 
BOOL My_GetVolumeInformationA(); 
BOOL My_GetVolumeInformationW(); 
