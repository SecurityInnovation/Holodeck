// Intercepts.cpp : Defines the entry point for the console application.
//
#include <winsock2.h>
#include "stdafx.h"

FARPROC disable = NULL;
FARPROC enable = NULL;

extern RtlLocalTimeToSystemTime_DEF RtlLocalTimeToSystemTime = NULL;
extern NtQuerySystemTime_DEF NtQuerySystemTime = NULL;
extern RtlTimeToSecondsSince1970_DEF RtlTimeToSecondsSince1970 = NULL;

BOOL loadLibraries( )
{
	HMODULE hModule = NULL;

	hModule = LoadLibrary( "NTDLL.dll" );
    
    if ( hModule == NULL )
    {
        printf( "COULD NOT LOAD NTDLL.DLL\n" );
        
        goto error;
    }

	////
    RtlLocalTimeToSystemTime = (RtlLocalTimeToSystemTime_DEF)GetProcAddress(hModule,"RtlLocalTimeToSystemTime");

    if ( RtlLocalTimeToSystemTime == NULL )
    {
        printf( "ERROR Getting \"RtlLocalTimeToSystemTime\" Function Address!" );
        goto error;
    }

	////
    NtQuerySystemTime = (NtQuerySystemTime_DEF)GetProcAddress(hModule,"NtQuerySystemTime");

    if ( NtQuerySystemTime == NULL )
    {
        printf( "ERROR Getting \"NtQuerySystemTime\" Function Address!" );
        goto error;
    }

	RtlTimeToSecondsSince1970 = (RtlTimeToSecondsSince1970_DEF)GetProcAddress(hModule,"NtQuerySystemTime");

    if ( RtlTimeToSecondsSince1970 == NULL )
    {
        printf( "ERROR Getting \"RtlTimeToSecondsSince1970\" Function Address!" );
        goto error;
    }

    return true;

error:
	return false;
}

BOOL printFunctionResult( BOOL bSuccess, CHAR * pszFunctionName )
{
    printf( "*** %s ", pszFunctionName );

    if ( bSuccess )
        printf( "PASSED!" );    
    else
	{
		printf( "FAILED!" );
		getchar();
	}

    printf( " ***\n");

    return bSuccess;
}

BOOL getInterceptionDisablers( )
{
    HMODULE hModule = NULL;

    // this will fail unless HEAT4DLL.dll is currently in the process space...
    hModule = GetModuleHandle("HEAT4DLL.dll");

    if ( hModule == NULL )
    {
        printf( "HEAT4.DLL MODULE NOT LOADED!\n" );
        printf( "Ensure that application is being intercepted by Holodeck!\n\n" );
        
        goto error;
    }

    disable = GetProcAddress(hModule,"disableInterceptionInCurrentThread");

    if ( disable == NULL )
    {
        printf( "ERROR Getting \"disableInterceptionInCurrentThread\" Function Address!" );
        goto error;
    }

    enable = GetProcAddress(hModule,"enableInterceptionInCurrentThread");

    if ( enable == NULL )
    {
        printf( "ERROR Getting \"enableInterceptionInCurrentThread\" Function Address!" );
        goto error;
    }

    return true;


error:
    return false;
}

void disableInterception( )
{
    if ( disable != NULL )
        disable( );
}

void enableInterception( )
{
    if ( enable != NULL )
        enable( );
}

int main( )
{
    printf( "<== HOLODECK INTERCEPT TESTER FOR WIN 32 Functions v1.0 ==>\n\n" );
    
    getInterceptionDisablers( );
	
	disableInterception( );
	loadLibraries( );
	enableInterception( );
    
    BOOL bSuccess = TRUE;


    /// The following functions are automatically generated ///

    
	bSuccess *= printFunctionResult( My_AllocateUserPhysicalPages( ), "AllocateUserPhysicalPages" );
    bSuccess *= printFunctionResult( My_bind( ), "bind" );
    bSuccess *= printFunctionResult( My_connect( ), "connect" );
    bSuccess *= printFunctionResult( My_CopyFileA( ), "CopyFileA" );
    bSuccess *= printFunctionResult( My_CopyFileExA( ), "CopyFileExA" );
    bSuccess *= printFunctionResult( My_CopyFileExW( ), "CopyFileExW" );
    bSuccess *= printFunctionResult( My_CopyFileW( ), "CopyFileW" );
    bSuccess *= printFunctionResult( My_CreateDirectoryA( ), "CreateDirectoryA" );
    bSuccess *= printFunctionResult( My_CreateDirectoryExA( ), "CreateDirectoryExA" );
    bSuccess *= printFunctionResult( My_CreateDirectoryExW( ), "CreateDirectoryExW" );
    bSuccess *= printFunctionResult( My_CreateDirectoryW( ), "CreateDirectoryW" );
    bSuccess *= printFunctionResult( My_CreateFileA( ), "CreateFileA" );
    bSuccess *= printFunctionResult( My_CreateFileW( ), "CreateFileW" );
   	bSuccess *= printFunctionResult( My_CreateProcessA( ), "CreateProcessA" );
	bSuccess *= printFunctionResult( My_CreateProcessW( ), "CreateProcessW" );
    bSuccess *= printFunctionResult( My_DeleteFileA( ), "DeleteFileA" );
    bSuccess *= printFunctionResult( My_DeleteFileW( ), "DeleteFileW" );
    bSuccess *= printFunctionResult( My_FlushFileBuffers( ), "FlushFileBuffers" );
    bSuccess *= printFunctionResult( My_FreeLibrary( ), "FreeLibrary" );
   	//bSuccess *= printFunctionResult( My_FreeLibraryAndExitThread( ), "FreeLibraryAndExitThread" );
    bSuccess *= printFunctionResult( My_FreeUserPhysicalPages( ), "FreeUserPhysicalPages" );
    bSuccess *= printFunctionResult( My_GetBinaryTypeA( ), "GetBinaryTypeA" );
    bSuccess *= printFunctionResult( My_GetBinaryTypeW( ), "GetBinaryTypeW" );
    bSuccess *= printFunctionResult( My_GetCurrentDirectoryA( ), "GetCurrentDirectoryA" );
    bSuccess *= printFunctionResult( My_GetCurrentDirectoryW( ), "GetCurrentDirectoryW" );
    bSuccess *= printFunctionResult( My_GetDiskFreeSpaceA( ), "GetDiskFreeSpaceA" );
    bSuccess *= printFunctionResult( My_GetDiskFreeSpaceExA( ), "GetDiskFreeSpaceExA" );
    bSuccess *= printFunctionResult( My_GetDiskFreeSpaceExW( ), "GetDiskFreeSpaceExW" );
    bSuccess *= printFunctionResult( My_GetDiskFreeSpaceW( ), "GetDiskFreeSpaceW" );
    bSuccess *= printFunctionResult( My_GetDriveTypeA( ), "GetDriveTypeA" );
    bSuccess *= printFunctionResult( My_GetDriveTypeW( ), "GetDriveTypeW" );
    bSuccess *= printFunctionResult( My_GetFileAttributesA( ), "GetFileAttributesA" );
   
//	bSuccess *= printFunctionResult( My_GetFileAttributesExA( ), "GetFileAttributesExA" );
//   bSuccess *= printFunctionResult( My_GetFileAttributesExW( ), "GetFileAttributesExW" );
    bSuccess *= printFunctionResult( My_GetFileAttributesW( ), "GetFileAttributesW" );
    bSuccess *= printFunctionResult( My_GetFileInformationByHandle( ), "GetFileInformationByHandle" );
    bSuccess *= printFunctionResult( My_GetFileSize( ), "GetFileSize" );
    bSuccess *= printFunctionResult( My_GetFileSizeEx( ), "GetFileSizeEx" );
    bSuccess *= printFunctionResult( My_GetFileType( ), "GetFileType" );
    bSuccess *= printFunctionResult( My_GetLogicalDrives( ), "GetLogicalDrives" );
    bSuccess *= printFunctionResult( My_GetLogicalDriveStringsA( ), "GetLogicalDriveStringsA" );
    bSuccess *= printFunctionResult( My_GetLogicalDriveStringsW( ), "GetLogicalDriveStringsW" );
   	bSuccess *= printFunctionResult( My_GlobalAlloc( ), "GlobalAlloc" );
    bSuccess *= printFunctionResult( My_GlobalFree( ), "GlobalFree" );
    bSuccess *= printFunctionResult( My_GlobalMemoryStatusEx( ), "GlobalMemoryStatusEx" );
    bSuccess *= printFunctionResult( My_HeapAlloc( ), "HeapAlloc" );
    bSuccess *= printFunctionResult( My_HeapCompact( ), "HeapCompact" );
    bSuccess *= printFunctionResult( My_HeapCreate( ), "HeapCreate" );
    bSuccess *= printFunctionResult( My_HeapDestroy( ), "HeapDestroy" );
    bSuccess *= printFunctionResult( My_HeapFree( ), "HeapFree" );
    bSuccess *= printFunctionResult( My_HeapLock( ), "HeapLock" );
    bSuccess *= printFunctionResult( My_HeapReAlloc( ), "HeapReAlloc" );
    bSuccess *= printFunctionResult( My_HeapSize( ), "HeapSize" );
    bSuccess *= printFunctionResult( My_HeapUnlock( ), "HeapUnlock" );
    bSuccess *= printFunctionResult( My_HeapValidate( ), "HeapValidate" );
    bSuccess *= printFunctionResult( My_ioctlsocket( ), "ioctlsocket" );
    bSuccess *= printFunctionResult( My_LoadLibraryA( ), "LoadLibraryA" );
    bSuccess *= printFunctionResult( My_LoadLibraryExA( ), "LoadLibraryExA" );
    bSuccess *= printFunctionResult( My_LoadLibraryExW( ), "LoadLibraryExW" );
    bSuccess *= printFunctionResult( My_LoadLibraryW( ), "LoadLibraryW" );
    bSuccess *= printFunctionResult( My_LocalAlloc( ), "LocalAlloc" );
    bSuccess *= printFunctionResult( My_LocalFree( ), "LocalFree" );
    bSuccess *= printFunctionResult( My_LocalReAlloc( ), "LocalReAlloc" );
    bSuccess *= printFunctionResult( My_MapUserPhysicalPages( ), "MapUserPhysicalPages" );
    bSuccess *= printFunctionResult( My_MapUserPhysicalPagesScatter( ), "MapUserPhysicalPagesScatter" );
    bSuccess *= printFunctionResult( My_MapViewOfFile( ), "MapViewOfFile" );
    bSuccess *= printFunctionResult( My_MapViewOfFileEx( ), "MapViewOfFileEx" );
    bSuccess *= printFunctionResult( My_MoveFileA( ), "MoveFileA" );
    bSuccess *= printFunctionResult( My_MoveFileExA( ), "MoveFileExA" );
    bSuccess *= printFunctionResult( My_MoveFileExW( ), "MoveFileExW" );
    bSuccess *= printFunctionResult( My_MoveFileW( ), "MoveFileW" );
    bSuccess *= printFunctionResult( My_MoveFileWithProgressA( ), "MoveFileWithProgressA" );
    bSuccess *= printFunctionResult( My_MoveFileWithProgressW( ), "MoveFileWithProgressW" );
    bSuccess *= printFunctionResult( My_ReadFile( ), "ReadFile" );
    bSuccess *= printFunctionResult( My_ReadFileEx( ), "ReadFileEx" );
    bSuccess *= printFunctionResult( My_ReadFileScatter( ), "ReadFileScatter" );
    bSuccess *= printFunctionResult( My_recv( ), "recv" );
    bSuccess *= printFunctionResult( My_RegCloseKey( ), "RegCloseKey" );
    bSuccess *= printFunctionResult( My_RegConnectRegistryA( ), "RegConnectRegistryA" );
    bSuccess *= printFunctionResult( My_RegConnectRegistryW( ), "RegConnectRegistryW" );
    bSuccess *= printFunctionResult( My_RegCreateKeyA( ), "RegCreateKeyA" );
    bSuccess *= printFunctionResult( My_RegCreateKeyExA( ), "RegCreateKeyExA" );
    bSuccess *= printFunctionResult( My_RegCreateKeyExW( ), "RegCreateKeyExW" );
    bSuccess *= printFunctionResult( My_RegCreateKeyW( ), "RegCreateKeyW" );
    bSuccess *= printFunctionResult( My_RegDeleteKeyA( ), "RegDeleteKeyA" );
    bSuccess *= printFunctionResult( My_RegDeleteKeyW( ), "RegDeleteKeyW" );
    bSuccess *= printFunctionResult( My_RegDeleteValueA( ), "RegDeleteValueA" );
    bSuccess *= printFunctionResult( My_RegDeleteValueW( ), "RegDeleteValueW" );
    bSuccess *= printFunctionResult( My_RegDisablePredefinedCache( ), "RegDisablePredefinedCache" );
    bSuccess *= printFunctionResult( My_RegEnumKeyA( ), "RegEnumKeyA" );
    bSuccess *= printFunctionResult( My_RegEnumKeyExA( ), "RegEnumKeyExA" );
    bSuccess *= printFunctionResult( My_RegEnumKeyExW( ), "RegEnumKeyExW" );
    bSuccess *= printFunctionResult( My_RegEnumKeyW( ), "RegEnumKeyW" );
    bSuccess *= printFunctionResult( My_RegEnumValueA( ), "RegEnumValueA" );
    bSuccess *= printFunctionResult( My_RegEnumValueW( ), "RegEnumValueW" );
    bSuccess *= printFunctionResult( My_RegFlushKey( ), "RegFlushKey" );
    bSuccess *= printFunctionResult( My_RegGetKeySecurity( ), "RegGetKeySecurity" );
    bSuccess *= printFunctionResult( My_RegLoadKeyA( ), "RegLoadKeyA" );
    bSuccess *= printFunctionResult( My_RegLoadKeyW( ), "RegLoadKeyW" );
    bSuccess *= printFunctionResult( My_RegNotifyChangeKeyValue( ), "RegNotifyChangeKeyValue" );
    bSuccess *= printFunctionResult( My_RegOpenCurrentUser( ), "RegOpenCurrentUser" );
    bSuccess *= printFunctionResult( My_RegOpenKeyA( ), "RegOpenKeyA" );
    bSuccess *= printFunctionResult( My_RegOpenKeyExA( ), "RegOpenKeyExA" );
    bSuccess *= printFunctionResult( My_RegOpenKeyExW( ), "RegOpenKeyExW" );
    bSuccess *= printFunctionResult( My_RegOpenKeyW( ), "RegOpenKeyW" );
    bSuccess *= printFunctionResult( My_RegOpenUserClassesRoot( ), "RegOpenUserClassesRoot" );
    bSuccess *= printFunctionResult( My_RegOverridePredefKey( ), "RegOverridePredefKey" );
    bSuccess *= printFunctionResult( My_RegQueryInfoKeyA( ), "RegQueryInfoKeyA" );
    bSuccess *= printFunctionResult( My_RegQueryInfoKeyW( ), "RegQueryInfoKeyW" );
    bSuccess *= printFunctionResult( My_RegQueryMultipleValuesA( ), "RegQueryMultipleValuesA" );
    bSuccess *= printFunctionResult( My_RegQueryMultipleValuesW( ), "RegQueryMultipleValuesW" );
    bSuccess *= printFunctionResult( My_RegQueryValueA( ), "RegQueryValueA" );
    bSuccess *= printFunctionResult( My_RegQueryValueW( ), "RegQueryValueW" );
    bSuccess *= printFunctionResult( My_RegQueryValueExA( ), "RegQueryValueExA" );
    bSuccess *= printFunctionResult( My_RegQueryValueExW( ), "RegQueryValueExW" );
    bSuccess *= printFunctionResult( My_RegReplaceKeyA( ), "RegReplaceKeyA" );
    bSuccess *= printFunctionResult( My_RegReplaceKeyW( ), "RegReplaceKeyW" );
    bSuccess *= printFunctionResult( My_RegRestoreKeyA( ), "RegRestoreKeyA" );
    bSuccess *= printFunctionResult( My_RegRestoreKeyW( ), "RegRestoreKeyW" );
    bSuccess *= printFunctionResult( My_RegSaveKeyA( ), "RegSaveKeyA" );
    bSuccess *= printFunctionResult( My_RegSaveKeyExA( ), "RegSaveKeyExA" );
    bSuccess *= printFunctionResult( My_RegSaveKeyExW( ), "RegSaveKeyExW" );
    bSuccess *= printFunctionResult( My_RegSaveKeyW( ), "RegSaveKeyW" );
    bSuccess *= printFunctionResult( My_RegSetKeySecurity( ), "RegSetKeySecurity" );
    bSuccess *= printFunctionResult( My_RegSetValueA( ), "RegSetValueA" );
    bSuccess *= printFunctionResult( My_RegSetValueExA( ), "RegSetValueExA" );
    bSuccess *= printFunctionResult( My_RegSetValueExW( ), "RegSetValueExW" );
    bSuccess *= printFunctionResult( My_RegSetValueW( ), "RegSetValueW" );
    bSuccess *= printFunctionResult( My_RegUnLoadKeyA( ), "RegUnLoadKeyA" );
    bSuccess *= printFunctionResult( My_RegUnLoadKeyW( ), "RegUnLoadKeyW" );
    bSuccess *= printFunctionResult( My_RemoveDirectoryA( ), "RemoveDirectoryA" );
    bSuccess *= printFunctionResult( My_RemoveDirectoryW( ), "RemoveDirectoryW" );
    bSuccess *= printFunctionResult( My_ReplaceFileA( ), "ReplaceFileA" );
    bSuccess *= printFunctionResult( My_ReplaceFileW( ), "ReplaceFileW" );
    bSuccess *= printFunctionResult( My_SearchPathA( ), "SearchPathA" );
    bSuccess *= printFunctionResult( My_SearchPathW( ), "SearchPathW" );
    bSuccess *= printFunctionResult( My_select( ), "select" );
    bSuccess *= printFunctionResult( My_send( ), "send" );
    bSuccess *= printFunctionResult( My_SetFileAttributesA( ), "SetFileAttributesA" );
    bSuccess *= printFunctionResult( My_SetFileAttributesW( ), "SetFileAttributesW" );
    bSuccess *= printFunctionResult( My_socket( ), "socket" );
    bSuccess *= printFunctionResult( My_VirtualAlloc( ), "VirtualAlloc" );
    bSuccess *= printFunctionResult( My_VirtualAllocEx( ), "VirtualAllocEx" );
    bSuccess *= printFunctionResult( My_VirtualFree( ), "VirtualFree" );
    bSuccess *= printFunctionResult( My_VirtualFreeEx( ), "VirtualFreeEx" );
    bSuccess *= printFunctionResult( My_VirtualLock( ), "VirtualLock" );
    bSuccess *= printFunctionResult( My_VirtualQuery( ), "VirtualQuery" );
    bSuccess *= printFunctionResult( My_VirtualQueryEx( ), "VirtualQueryEx" );
    bSuccess *= printFunctionResult( My_VirtualUnlock( ), "VirtualUnlock" );
    bSuccess *= printFunctionResult( My_WriteFile( ), "WriteFile" );
    //bSuccess *= printFunctionResult( My_WriteFileEx( ), "WriteFileEx" );
    bSuccess *= printFunctionResult( My_WriteFileGather( ), "WriteFileGather" );
    bSuccess *= printFunctionResult( My_WSAConnect( ), "WSAConnect" );
    bSuccess *= printFunctionResult( My_WSAEventSelect( ), "WSAEventSelect" );
    bSuccess *= printFunctionResult( My_WSAIoctl( ), "WSAIoctl" );
    bSuccess *= printFunctionResult( My_WSARecv( ), "WSARecv" );
    bSuccess *= printFunctionResult( My_WSASend( ), "WSASend" );
    bSuccess *= printFunctionResult( My_WSASocketA( ), "WSASocketA" );
    bSuccess *= printFunctionResult( My_WSASocketW( ), "WSASocketW" );
    bSuccess *= printFunctionResult( My_WSAStartup( ), "WSAStartup" );
    bSuccess *= printFunctionResult( My_AllocConsole( ), "AllocConsole" );
    bSuccess *= printFunctionResult( My_AreFileApisANSI( ), "AreFileApisANSI" );
    bSuccess *= printFunctionResult( My_AttachConsole( ), "AttachConsole" );
    bSuccess *= printFunctionResult( My_BackupRead( ), "BackupRead" );
    bSuccess *= printFunctionResult( My_BackupSeek( ), "BackupSeek" );
    bSuccess *= printFunctionResult( My_BackupWrite( ), "BackupWrite" );
    bSuccess *= printFunctionResult( My_CancelIo( ), "CancelIo" );
    bSuccess *= printFunctionResult( My_CloseHandle( ), "CloseHandle" );
    bSuccess *= printFunctionResult( My_CopyLZFile( ), "CopyLZFile" );
    bSuccess *= printFunctionResult( My_CreateFiber( ), "CreateFiber" );
    bSuccess *= printFunctionResult( My_CreateFiberEx( ), "CreateFiberEx" );
    bSuccess *= printFunctionResult( My_CreateFileMappingA( ), "CreateFileMappingA" );
    bSuccess *= printFunctionResult( My_CreateFileMappingW( ), "CreateFileMappingW" );
    bSuccess *= printFunctionResult( My_CreateHardLinkA( ), "CreateHardLinkA" );
    bSuccess *= printFunctionResult( My_CreateHardLinkW( ), "CreateHardLinkW" );
   // bSuccess *= printFunctionResult( My_CreateMemoryResourceNotification( ), "CreateMemoryResourceNotification" );
    //bSuccess *= printFunctionResult( My_CreateThread( ), "CreateThread" );
    bSuccess *= printFunctionResult( My_CreateToolhelp32Snapshot( ), "CreateToolhelp32Snapshot" );
    bSuccess *= printFunctionResult( My_ContinueDebugEvent( ), "ContinueDebugEvent" );
    bSuccess *= printFunctionResult( My_DebugActiveProcess( ), "DebugActiveProcess" );
    bSuccess *= printFunctionResult( My_DebugActiveProcessStop( ), "DebugActiveProcessStop" );
    //bSuccess *= printFunctionResult( My_DebugBreak( ), "DebugBreak" );
    bSuccess *= printFunctionResult( My_DebugBreakProcess( ), "DebugBreakProcess" );
    bSuccess *= printFunctionResult( My_DebugSetProcessKillOnExit( ), "DebugSetProcessKillOnExit" );
    //bSuccess *= printFunctionResult( My_FatalAppExitA( ), "FatalAppExitA" );
    //bSuccess *= printFunctionResult( My_FatalAppExitW( ), "FatalAppExitW" );
    //bSuccess *= printFunctionResult( My_FatalExit( ), "FatalExit" );
    bSuccess *= printFunctionResult( My_DeleteFiber( ), "DeleteFiber" );
    bSuccess *= printFunctionResult( My_DisableThreadLibraryCalls( ), "DisableThreadLibraryCalls" );
    bSuccess *= printFunctionResult( My_DnsHostnameToComputerNameA( ), "DnsHostnameToComputerNameA" );
    bSuccess *= printFunctionResult( My_DnsHostnameToComputerNameW( ), "DnsHostnameToComputerNameW" );
    //bSuccess *= printFunctionResult( My_ExitProcess( ), "ExitProcess" );
    //bSuccess *= printFunctionResult( My_FindFirstFileA( ), "FindFirstFileA" );
    //bSuccess *= printFunctionResult( My_FindFirstFileW( ), "FindFirstFileW" );
    /*bSuccess *= printFunctionResult( My_FindFirstFileExA( ), "FindFirstFileExA" );
    bSuccess *= printFunctionResult( My_FindFirstFileExW( ), "FindFirstFileExW" );
    bSuccess *= printFunctionResult( My_FindNextFileA( ), "FindNextFileA" );
    bSuccess *= printFunctionResult( My_FindNextFileW( ), "FindNextFileW" );*/

    bSuccess *= printFunctionResult( My_GetCompressedFileSizeA( ), "GetCompressedFileSizeA" );
    bSuccess *= printFunctionResult( My_GetCompressedFileSizeW( ), "GetCompressedFileSizeW" );
    bSuccess *= printFunctionResult( My_GetExitCodeProcess( ), "GetExitCodeProcess" );
    bSuccess *= printFunctionResult( My_GetExitCodeThread( ), "GetExitCodeThread" );
    bSuccess *= printFunctionResult( My_GetFullPathNameA( ), "GetFullPathNameA" );
    bSuccess *= printFunctionResult( My_GetFullPathNameW( ), "GetFullPathNameW" );
    bSuccess *= printFunctionResult( My_GetProcessHeap( ), "GetProcessHeap" );
    bSuccess *= printFunctionResult( My_GetProcessHeaps( ), "GetProcessHeaps" );
    bSuccess *= printFunctionResult( My_GetProcessIoCounters( ), "GetProcessIoCounters" );
    bSuccess *= printFunctionResult( My_GetProcessTimes( ), "GetProcessTimes" );
    bSuccess *= printFunctionResult( My_GetProcessWorkingSetSize( ), "GetProcessWorkingSetSize" );
    bSuccess *= printFunctionResult( My_GetStartupInfoA( ), "GetStartupInfoA" );
    bSuccess *= printFunctionResult( My_GetStartupInfoW( ), "GetStartupInfoW" );
    bSuccess *= printFunctionResult( My_GetSystemDirectoryA( ), "GetSystemDirectoryA" );
    bSuccess *= printFunctionResult( My_GetSystemDirectoryW( ), "GetSystemDirectoryW" );
    bSuccess *= printFunctionResult( My_GetSystemWindowsDirectoryA( ), "GetSystemWindowsDirectoryA" );
    bSuccess *= printFunctionResult( My_GetSystemWindowsDirectoryW( ), "GetSystemWindowsDirectoryW" );
    bSuccess *= printFunctionResult( My_GetWriteWatch( ), "GetWriteWatch" );
    bSuccess *= printFunctionResult( My_GlobalMemoryStatus( ), "GlobalMemoryStatus" );
    bSuccess *= printFunctionResult( My_GlobalReAlloc( ), "GlobalReAlloc" );
    bSuccess *= printFunctionResult( My_LockFile( ), "LockFile" );
    bSuccess *= printFunctionResult( My_LockFileEx( ), "LockFileEx" );
    bSuccess *= printFunctionResult( My_LZClose( ), "LZClose" );
    bSuccess *= printFunctionResult( My_LZCopy( ), "LZCopy" );
    bSuccess *= printFunctionResult( My_LZDone( ), "LZDone" );
    bSuccess *= printFunctionResult( My_LZOpenFileA( ), "LZOpenFileA" );
    bSuccess *= printFunctionResult( My_LZOpenFileW( ), "LZOpenFileW" );
    bSuccess *= printFunctionResult( My_LZRead( ), "LZRead" );
    bSuccess *= printFunctionResult( My_LZSeek( ), "LZSeek" );
    bSuccess *= printFunctionResult( My_LoadModule( ), "LoadModule" );
    bSuccess *= printFunctionResult( My_OpenFile( ), "OpenFile" );
    bSuccess *= printFunctionResult( My_QueryMemoryResourceNotification( ), "QueryMemoryResourceNotification" );
    bSuccess *= printFunctionResult( My_SetFileApisToANSI( ), "SetFileApisToANSI" );
    bSuccess *= printFunctionResult( My_SetFileApisToOEM( ), "SetFileApisToOEM" );
    bSuccess *= printFunctionResult( My_SetFilePointer( ), "SetFilePointer" );
    bSuccess *= printFunctionResult( My_SetFileShortNameA( ), "SetFileShortNameA" );
    bSuccess *= printFunctionResult( My_SetFileShortNameW( ), "SetFileShortNameW" );
    bSuccess *= printFunctionResult( My_SetFileValidData( ), "SetFileValidData" );
    bSuccess *= printFunctionResult( My_SetEndOfFile( ), "SetEndOfFile" );
    bSuccess *= printFunctionResult( My_SetPriorityClass( ), "SetPriorityClass" );
    bSuccess *= printFunctionResult( My_SetProcessShutdownParameters( ), "SetProcessShutdownParameters" );
    bSuccess *= printFunctionResult( My_SetProcessWorkingSetSize( ), "SetProcessWorkingSetSize" );
    bSuccess *= printFunctionResult( My_TerminateProcess( ), "TerminateProcess" );
    bSuccess *= printFunctionResult( My_TerminateThread( ), "TerminateThread" );
    bSuccess *= printFunctionResult( My_TlsAlloc( ), "TlsAlloc" );
    bSuccess *= printFunctionResult( My_TlsFree( ), "TlsFree" );
    bSuccess *= printFunctionResult( My_UnlockFile( ), "UnlockFile" );
    bSuccess *= printFunctionResult( My_UnlockFileEx( ), "UnlockFileEx" );
    bSuccess *= printFunctionResult( My_VirtualProtect( ), "VirtualProtect" );
    bSuccess *= printFunctionResult( My_VirtualProtectEx( ), "VirtualProtectEx" );
    bSuccess *= printFunctionResult( My_WriteProcessMemory( ), "WriteProcessMemory" );
    bSuccess *= printFunctionResult( My__hread( ), "_hread" );
    bSuccess *= printFunctionResult( My__hwrite( ), "_hwrite" );
    bSuccess *= printFunctionResult( My__lclose( ), "_lclose" );
    bSuccess *= printFunctionResult( My__lcreat( ), "_lcreat" );
    bSuccess *= printFunctionResult( My__lread( ), "_lread" );
    bSuccess *= printFunctionResult( My__lwrite( ), "_lwrite" );
    bSuccess *= printFunctionResult( My__llseek( ), "_llseek" );
    bSuccess *= printFunctionResult( My__lopen( ), "_lopen" );
    bSuccess *= printFunctionResult( My_BackupEventLogA( ), "BackupEventLogA" );
    bSuccess *= printFunctionResult( My_BackupEventLogW( ), "BackupEventLogW" );
    bSuccess *= printFunctionResult( My_ClearEventLogA( ), "ClearEventLogA" );
    bSuccess *= printFunctionResult( My_ClearEventLogW( ), "ClearEventLogW" );
    bSuccess *= printFunctionResult( My_CloseEventLog( ), "CloseEventLog" );
    bSuccess *= printFunctionResult( My_CreateProcessAsUserA( ), "CreateProcessAsUserA" );
    bSuccess *= printFunctionResult( My_CreateProcessAsUserW( ), "CreateProcessAsUserW" );
    bSuccess *= printFunctionResult( My_CreateProcessWithLogonW( ), "CreateProcessWithLogonW" );
    bSuccess *= printFunctionResult( My_DecryptFileA( ), "DecryptFileA" );
    bSuccess *= printFunctionResult( My_DecryptFileW( ), "DecryptFileW" );
    bSuccess *= printFunctionResult( My_EncryptFileA( ), "EncryptFileA" );
    bSuccess *= printFunctionResult( My_EncryptFileW( ), "EncryptFileW" );
    bSuccess *= printFunctionResult( My_FileEncryptionStatusA( ), "FileEncryptionStatusA" );
    bSuccess *= printFunctionResult( My_FileEncryptionStatusW( ), "FileEncryptionStatusW" );
    bSuccess *= printFunctionResult( My_GetFileSecurityA( ), "GetFileSecurityA" );
    bSuccess *= printFunctionResult( My_GetFileSecurityW( ), "GetFileSecurityW" );
    bSuccess *= printFunctionResult( My_OpenBackupEventLogA( ), "OpenBackupEventLogA" );
    bSuccess *= printFunctionResult( My_OpenBackupEventLogW( ), "OpenBackupEventLogW" );
    bSuccess *= printFunctionResult( My_OpenEventLogA( ), "OpenEventLogA" );
    bSuccess *= printFunctionResult( My_OpenEventLogW( ), "OpenEventLogW" );
    bSuccess *= printFunctionResult( My_QueryUsersOnEncryptedFile( ), "QueryUsersOnEncryptedFile" );
    bSuccess *= printFunctionResult( My_RemoveUsersFromEncryptedFile( ), "RemoveUsersFromEncryptedFile" );
    bSuccess *= printFunctionResult( My_SaferRecordEventLogEntry( ), "SaferRecordEventLogEntry" );
    //bSuccess *= printFunctionResult( My_SaferiIsExecutableFileType( ), "SaferiIsExecutableFileType" );*/
    bSuccess *= printFunctionResult( My_SetFileSecurityA( ), "SetFileSecurityA" );
    bSuccess *= printFunctionResult( My_SetFileSecurityW( ), "SetFileSecurityW" );
    bSuccess *= printFunctionResult( My_CompareFileTime( ), "CompareFileTime" );
    bSuccess *= printFunctionResult( My_DosDateTimeToFileTime( ), "DosDateTimeToFileTime" );
    bSuccess *= printFunctionResult( My_FileTimeToDosDateTime( ), "FileTimeToDosDateTime" );
    bSuccess *= printFunctionResult( My_FileTimeToLocalFileTime( ), "FileTimeToLocalFileTime" );
    bSuccess *= printFunctionResult( My_FileTimeToSystemTime( ), "FileTimeToSystemTime" );
    bSuccess *= printFunctionResult( My_GetFileTime( ), "GetFileTime" );
    bSuccess *= printFunctionResult( My_GetLocalTime( ), "GetLocalTime" );
    bSuccess *= printFunctionResult( My_GetSystemTime( ), "GetSystemTime" );
    bSuccess *= printFunctionResult( My_GetSystemTimeAdjustment( ), "GetSystemTimeAdjustment" );
    bSuccess *= printFunctionResult( My_GetSystemTimeAsFileTime( ), "GetSystemTimeAsFileTime" );
    bSuccess *= printFunctionResult( My_GetTickCount( ), "GetTickCount" );
    bSuccess *= printFunctionResult( My_GetTimeFormatA( ), "GetTimeFormatA" );
    bSuccess *= printFunctionResult( My_GetTimeFormatW( ), "GetTimeFormatW" );
    bSuccess *= printFunctionResult( My_GetTimeZoneInformation( ), "GetTimeZoneInformation" );
    bSuccess *= printFunctionResult( My_LocalFileTimeToFileTime( ), "LocalFileTimeToFileTime" );
    bSuccess *= printFunctionResult( My_SetFileTime( ), "SetFileTime" );
    bSuccess *= printFunctionResult( My_SetLocalTime( ), "SetLocalTime" );
    bSuccess *= printFunctionResult( My_SetSystemTime( ), "SetSystemTime" );
    bSuccess *= printFunctionResult( My_SetSystemTimeAdjustment( ), "SetSystemTimeAdjustment" );
    bSuccess *= printFunctionResult( My_SetTimeZoneInformation( ), "SetTimeZoneInformation" );
    bSuccess *= printFunctionResult( My_SystemTimeToFileTime( ), "SystemTimeToFileTime" );
    bSuccess *= printFunctionResult( My_SystemTimeToTzSpecificLocalTime( ), "SystemTimeToTzSpecificLocalTime" );
    bSuccess *= printFunctionResult( My_TzSpecificLocalTimeToSystemTime( ), "TzSpecificLocalTimeToSystemTime" );
    bSuccess *= printFunctionResult( My_QueryPerformanceCounter( ), "QueryPerformanceCounter" );
    bSuccess *= printFunctionResult( My_QueryPerformanceFrequency( ), "QueryPerformanceFrequency" );
    bSuccess *= printFunctionResult( My_recvfrom( ), "recvfrom" );
    bSuccess *= printFunctionResult( My_sendto( ), "sendto" );
    bSuccess *= printFunctionResult( My_setsockopt( ), "setsockopt" );
    bSuccess *= printFunctionResult( My_getsockopt( ), "getsockopt" );
    bSuccess *= printFunctionResult( My_listen( ), "listen" );
    bSuccess *= printFunctionResult( My_gethostbyaddr( ), "gethostbyaddr" );
    bSuccess *= printFunctionResult( My_gethostbyname( ), "gethostbyname" );
    bSuccess *= printFunctionResult( My_accept( ), "accept" );
    bSuccess *= printFunctionResult( My_WSARecvFrom( ), "WSARecvFrom" );
    bSuccess *= printFunctionResult( My_WSASendDisconnect( ), "WSASendDisconnect" );
    bSuccess *= printFunctionResult( My_WSAJoinLeaf( ), "WSAJoinLeaf" );
    bSuccess *= printFunctionResult( My_WSARecvDisconnect( ), "WSARecvDisconnect" );
    bSuccess *= printFunctionResult( My_WSAProviderConfigChange( ), "WSAProviderConfigChange" );
    bSuccess *= printFunctionResult( My_WSADuplicateSocketA( ), "WSADuplicateSocketA" );
    bSuccess *= printFunctionResult( My_WSADuplicateSocketW( ), "WSADuplicateSocketW" );
    bSuccess *= printFunctionResult( My_WSAAsyncSelect( ), "WSAAsyncSelect" );
    bSuccess *= printFunctionResult( My_WSACancelAsyncRequest( ), "WSACancelAsyncRequest" );
    bSuccess *= printFunctionResult( My_WSACleanup( ), "WSACleanup" );
    bSuccess *= printFunctionResult( My_WSAAccept( ), "WSAAccept" );
    bSuccess *= printFunctionResult( My_WSAAsyncGetHostByAddr( ), "WSAAsyncGetHostByAddr" );
    bSuccess *= printFunctionResult( My_WSAAsyncGetHostByName( ), "WSAAsyncGetHostByName" );
    bSuccess *= printFunctionResult( My_WSAAsyncGetServByName( ), "WSAAsyncGetServByName" );
    bSuccess *= printFunctionResult( My_WSAAsyncGetServByPort( ), "WSAAsyncGetServByPort" );
    bSuccess *= printFunctionResult( My_shutdown( ), "shutdown" );
    bSuccess *= printFunctionResult( My_WSASendTo( ), "WSASendTo" );
    bSuccess *= printFunctionResult( My_GetProcAddress( ), "GetProcAddress" );
    bSuccess *= printFunctionResult( My_GetModuleHandleA( ), "GetModuleHandleA" );
    bSuccess *= printFunctionResult( My_GetModuleHandleW( ), "GetModuleHandleW" );
    bSuccess *= printFunctionResult( My_DeviceIoControl( ), "DeviceIoControl" );
    bSuccess *= printFunctionResult( My_GetVersionExA( ), "GetVersionExA" );
    bSuccess *= printFunctionResult( My_GetVersionExW( ), "GetVersionExW" );
    bSuccess *= printFunctionResult( My_GetSystemInfo( ), "GetSystemInfo" );
    bSuccess *= printFunctionResult( My_GetVolumeInformationA( ), "GetVolumeInformationA" );
    bSuccess *= printFunctionResult( My_GetVolumeInformationW( ), "GetVolumeInformationW" );

    

    printf("\n\n");

    if ( bSuccess )
        printf( "===>TEST PASSED!<===\n" );
    else
        printf( "===>TEST FAILED!<===\n" );

    printf( "\n<< PRESS ENTER TO CONTINUE >>\n" );
    getchar( );

    return 0;

}









