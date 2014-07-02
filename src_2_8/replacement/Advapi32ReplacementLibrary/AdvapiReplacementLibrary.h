//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		AdvapiReplacementLibrary.h
//
// DESCRIPTION: Contains replacement library prototypes for advapi32.dll
//
//=========================================================================
// Modification History
//
// Generated 05/04/2004 04:51:29 PM
//*************************************************************************
#pragma once

#include <windows.h>
#include <winsafer.h>
#include "ReplacementLibrary.h"
#include "Log.h"

using namespace Replacement;
using namespace Log;

// function pointer definitions
typedef WINADVAPI LONG (APIENTRY *RegCloseKeyFunction)(HKEY);
typedef WINADVAPI LONG (APIENTRY *RegConnectRegistryAFunction)(LPCSTR, HKEY, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegConnectRegistryWFunction)(LPCWSTR, HKEY, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegCreateKeyAFunction)(HKEY, LPCSTR, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegCreateKeyExAFunction)(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegCreateKeyExWFunction)(HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegCreateKeyWFunction)(HKEY, LPCWSTR, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegDeleteKeyAFunction)(HKEY, LPCSTR);
typedef WINADVAPI LONG (APIENTRY *RegDeleteKeyWFunction)(HKEY, LPCWSTR);
typedef WINADVAPI LONG (APIENTRY *RegDeleteValueAFunction)(HKEY, LPCSTR);
typedef WINADVAPI LONG (APIENTRY *RegDeleteValueWFunction)(HKEY, LPCWSTR);
typedef WINADVAPI LONG (APIENTRY *RegDisablePredefinedCacheFunction)();
typedef WINADVAPI LONG (APIENTRY *RegEnumKeyAFunction)(HKEY, DWORD, LPSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegEnumKeyExAFunction)(HKEY, DWORD, LPSTR, LPDWORD, LPDWORD, LPSTR, LPDWORD, PFILETIME);
typedef WINADVAPI LONG (APIENTRY *RegEnumKeyExWFunction)(HKEY, DWORD, LPWSTR, LPDWORD, LPDWORD, LPWSTR, LPDWORD, PFILETIME);
typedef WINADVAPI LONG (APIENTRY *RegEnumKeyWFunction)(HKEY, DWORD, LPWSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegEnumValueAFunction)(HKEY, DWORD, LPSTR, LPDWORD, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegEnumValueWFunction)(HKEY, DWORD, LPWSTR, LPDWORD, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegFlushKeyFunction)(HKEY);
typedef WINADVAPI LONG (APIENTRY *RegGetKeySecurityFunction)(HKEY, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegLoadKeyAFunction)(HKEY, LPCSTR, LPCSTR);
typedef WINADVAPI LONG (APIENTRY *RegLoadKeyWFunction)(HKEY, LPCWSTR, LPCWSTR);
typedef WINADVAPI LONG (APIENTRY *RegNotifyChangeKeyValueFunction)(HKEY, BOOL, DWORD, HANDLE, BOOL);
typedef WINADVAPI LONG (APIENTRY *RegOpenCurrentUserFunction)(REGSAM, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOpenKeyAFunction)(HKEY, LPCSTR, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOpenKeyExAFunction)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOpenKeyExWFunction)(HKEY, LPCWSTR, DWORD, REGSAM, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOpenKeyWFunction)(HKEY, LPCWSTR, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOpenUserClassesRootFunction)(HANDLE, DWORD, REGSAM, PHKEY);
typedef WINADVAPI LONG (APIENTRY *RegOverridePredefKeyFunction)(HKEY, HKEY);
typedef WINADVAPI LONG (APIENTRY *RegQueryInfoKeyAFunction)(HKEY, LPSTR, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, PFILETIME);
typedef WINADVAPI LONG (APIENTRY *RegQueryInfoKeyWFunction)(HKEY, LPWSTR, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, LPDWORD, PFILETIME);
typedef WINADVAPI LONG (APIENTRY *RegQueryMultipleValuesAFunction)(HKEY, PVALENTA, DWORD, LPSTR, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegQueryMultipleValuesWFunction)(HKEY, PVALENTW, DWORD, LPWSTR, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegQueryValueAFunction)(HKEY, LPCSTR, LPSTR, PLONG);
typedef WINADVAPI LONG (APIENTRY *RegQueryValueWFunction)(HKEY, LPCWSTR, LPWSTR, PLONG);
typedef WINADVAPI LONG (APIENTRY *RegQueryValueExAFunction)(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegQueryValueExWFunction)(HKEY, LPCWSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
typedef WINADVAPI LONG (APIENTRY *RegReplaceKeyAFunction)(HKEY, LPCSTR, LPCSTR, LPCSTR);
typedef WINADVAPI LONG (APIENTRY *RegReplaceKeyWFunction)(HKEY, LPCWSTR, LPCWSTR, LPCWSTR);
typedef WINADVAPI LONG (APIENTRY *RegRestoreKeyAFunction)(HKEY, LPCSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegRestoreKeyWFunction)(HKEY, LPCWSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSaveKeyAFunction)(HKEY, LPCSTR, LPSECURITY_ATTRIBUTES);
typedef WINADVAPI LONG (APIENTRY *RegSaveKeyExAFunction)(HKEY, LPCSTR, LPSECURITY_ATTRIBUTES, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSaveKeyExWFunction)(HKEY, LPCWSTR, LPSECURITY_ATTRIBUTES, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSaveKeyWFunction)(HKEY, LPCWSTR, LPSECURITY_ATTRIBUTES);
typedef WINADVAPI LONG (APIENTRY *RegSetKeySecurityFunction)(HKEY, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR);
typedef WINADVAPI LONG (APIENTRY *RegSetValueAFunction)(HKEY, LPCSTR, DWORD, LPCSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSetValueExAFunction)(HKEY, LPCSTR, DWORD, DWORD, CONST BYTE *, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSetValueExWFunction)(HKEY, LPCWSTR, DWORD, DWORD, CONST BYTE *, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegSetValueWFunction)(HKEY, LPCWSTR, DWORD, LPCWSTR, DWORD);
typedef WINADVAPI LONG (APIENTRY *RegUnLoadKeyAFunction)(HKEY, LPCSTR);
typedef WINADVAPI LONG (APIENTRY *RegUnLoadKeyWFunction)(HKEY, LPCWSTR);
typedef WINADVAPI BOOL (WINAPI *BackupEventLogAFunction)(HANDLE, LPCSTR);
typedef WINADVAPI BOOL (WINAPI *BackupEventLogWFunction)(HANDLE, LPCWSTR);
typedef WINADVAPI BOOL (WINAPI *ClearEventLogAFunction)(HANDLE, LPCSTR);
typedef WINADVAPI BOOL (WINAPI *ClearEventLogWFunction)(HANDLE, LPCWSTR);
typedef WINADVAPI BOOL (WINAPI *CloseEventLogFunction)(HANDLE);
typedef WINADVAPI BOOL (WINAPI *CreateProcessAsUserAFunction)(HANDLE, LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessAsUserWFunction)(HANDLE, LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessWithLogonWFunction)(LPCWSTR, LPCWSTR, LPCWSTR, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessWithTokenWFunction)(HANDLE, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *DecryptFileAFunction)(LPCSTR, DWORD);
typedef WINADVAPI BOOL (WINAPI *DecryptFileWFunction)(LPCWSTR, DWORD);
typedef WINADVAPI BOOL (WINAPI *EncryptFileAFunction)(LPCSTR);
typedef WINADVAPI BOOL (WINAPI *EncryptFileWFunction)(LPCWSTR);
typedef WINADVAPI BOOL (WINAPI *FileEncryptionStatusAFunction)(LPCSTR, LPDWORD);
typedef WINADVAPI BOOL (WINAPI *FileEncryptionStatusWFunction)(LPCWSTR, LPDWORD);
typedef WINADVAPI BOOL (WINAPI *GetFileSecurityAFunction)(LPCSTR, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR, DWORD, LPDWORD);
typedef WINADVAPI BOOL (WINAPI *GetFileSecurityWFunction)(LPCWSTR, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR, DWORD, LPDWORD);
typedef WINADVAPI HANDLE (WINAPI *OpenBackupEventLogAFunction)(LPCSTR, LPCSTR);
typedef WINADVAPI HANDLE (WINAPI *OpenBackupEventLogWFunction)(LPCWSTR, LPCWSTR);
typedef WINADVAPI HANDLE (WINAPI *OpenEventLogAFunction)(LPCSTR, LPCSTR);
typedef WINADVAPI HANDLE (WINAPI *OpenEventLogWFunction)(LPCWSTR, LPCWSTR);
typedef WINADVAPI DWORD (WINAPI *QueryUsersOnEncryptedFileFunction)(LPCWSTR, PENCRYPTION_CERTIFICATE_HASH_LIST *);
typedef WINADVAPI DWORD (WINAPI *RemoveUsersFromEncryptedFileFunction)(LPCWSTR, PENCRYPTION_CERTIFICATE_HASH_LIST);
typedef WINADVAPI BOOL (WINAPI *SaferRecordEventLogEntryFunction)(SAFER_LEVEL_HANDLE, LPCWSTR, LPVOID);
typedef WINADVAPI BOOL (WINAPI *SaferiIsExecutableFileTypeFunction)(LPCWSTR, BOOLEAN);
typedef WINADVAPI BOOL (WINAPI *SetFileSecurityAFunction)(LPCSTR, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR);
typedef WINADVAPI BOOL (WINAPI *SetFileSecurityWFunction)(LPCWSTR, SECURITY_INFORMATION, PSECURITY_DESCRIPTOR);
typedef WINADVAPI BOOL (WINAPI *SetSecurityDescriptorDaclFunction)(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL);

// original function pointers
RegCloseKeyFunction realRegCloseKey = NULL;
RegConnectRegistryAFunction realRegConnectRegistryA = NULL;
RegConnectRegistryWFunction realRegConnectRegistryW = NULL;
RegCreateKeyAFunction realRegCreateKeyA = NULL;
RegCreateKeyExAFunction realRegCreateKeyExA = NULL;
RegCreateKeyExWFunction realRegCreateKeyExW = NULL;
RegCreateKeyWFunction realRegCreateKeyW = NULL;
RegDeleteKeyAFunction realRegDeleteKeyA = NULL;
RegDeleteKeyWFunction realRegDeleteKeyW = NULL;
RegDeleteValueAFunction realRegDeleteValueA = NULL;
RegDeleteValueWFunction realRegDeleteValueW = NULL;
RegDisablePredefinedCacheFunction realRegDisablePredefinedCache = NULL;
RegEnumKeyAFunction realRegEnumKeyA = NULL;
RegEnumKeyExAFunction realRegEnumKeyExA = NULL;
RegEnumKeyExWFunction realRegEnumKeyExW = NULL;
RegEnumKeyWFunction realRegEnumKeyW = NULL;
RegEnumValueAFunction realRegEnumValueA = NULL;
RegEnumValueWFunction realRegEnumValueW = NULL;
RegFlushKeyFunction realRegFlushKey = NULL;
RegGetKeySecurityFunction realRegGetKeySecurity = NULL;
RegLoadKeyAFunction realRegLoadKeyA = NULL;
RegLoadKeyWFunction realRegLoadKeyW = NULL;
RegNotifyChangeKeyValueFunction realRegNotifyChangeKeyValue = NULL;
RegOpenCurrentUserFunction realRegOpenCurrentUser = NULL;
RegOpenKeyAFunction realRegOpenKeyA = NULL;
RegOpenKeyExAFunction realRegOpenKeyExA = NULL;
RegOpenKeyExWFunction realRegOpenKeyExW = NULL;
RegOpenKeyWFunction realRegOpenKeyW = NULL;
RegOpenUserClassesRootFunction realRegOpenUserClassesRoot = NULL;
RegOverridePredefKeyFunction realRegOverridePredefKey = NULL;
RegQueryInfoKeyAFunction realRegQueryInfoKeyA = NULL;
RegQueryInfoKeyWFunction realRegQueryInfoKeyW = NULL;
RegQueryMultipleValuesAFunction realRegQueryMultipleValuesA = NULL;
RegQueryMultipleValuesWFunction realRegQueryMultipleValuesW = NULL;
RegQueryValueAFunction realRegQueryValueA = NULL;
RegQueryValueWFunction realRegQueryValueW = NULL;
RegQueryValueExAFunction realRegQueryValueExA = NULL;
RegQueryValueExWFunction realRegQueryValueExW = NULL;
RegReplaceKeyAFunction realRegReplaceKeyA = NULL;
RegReplaceKeyWFunction realRegReplaceKeyW = NULL;
RegRestoreKeyAFunction realRegRestoreKeyA = NULL;
RegRestoreKeyWFunction realRegRestoreKeyW = NULL;
RegSaveKeyAFunction realRegSaveKeyA = NULL;
RegSaveKeyExAFunction realRegSaveKeyExA = NULL;
RegSaveKeyExWFunction realRegSaveKeyExW = NULL;
RegSaveKeyWFunction realRegSaveKeyW = NULL;
RegSetKeySecurityFunction realRegSetKeySecurity = NULL;
RegSetValueAFunction realRegSetValueA = NULL;
RegSetValueExAFunction realRegSetValueExA = NULL;
RegSetValueExWFunction realRegSetValueExW = NULL;
RegSetValueWFunction realRegSetValueW = NULL;
RegUnLoadKeyAFunction realRegUnLoadKeyA = NULL;
RegUnLoadKeyWFunction realRegUnLoadKeyW = NULL;
BackupEventLogAFunction realBackupEventLogA = NULL;
BackupEventLogWFunction realBackupEventLogW = NULL;
ClearEventLogAFunction realClearEventLogA = NULL;
ClearEventLogWFunction realClearEventLogW = NULL;
CloseEventLogFunction realCloseEventLog = NULL;
CreateProcessAsUserAFunction realCreateProcessAsUserA = NULL;
CreateProcessAsUserWFunction realCreateProcessAsUserW = NULL;
CreateProcessWithLogonWFunction realCreateProcessWithLogonW = NULL;
CreateProcessWithTokenWFunction realCreateProcessWithTokenW = NULL;
DecryptFileAFunction realDecryptFileA = NULL;
DecryptFileWFunction realDecryptFileW = NULL;
EncryptFileAFunction realEncryptFileA = NULL;
EncryptFileWFunction realEncryptFileW = NULL;
FileEncryptionStatusAFunction realFileEncryptionStatusA = NULL;
FileEncryptionStatusWFunction realFileEncryptionStatusW = NULL;
GetFileSecurityAFunction realGetFileSecurityA = NULL;
GetFileSecurityWFunction realGetFileSecurityW = NULL;
OpenBackupEventLogAFunction realOpenBackupEventLogA = NULL;
OpenBackupEventLogWFunction realOpenBackupEventLogW = NULL;
OpenEventLogAFunction realOpenEventLogA = NULL;
OpenEventLogWFunction realOpenEventLogW = NULL;
QueryUsersOnEncryptedFileFunction realQueryUsersOnEncryptedFile = NULL;
RemoveUsersFromEncryptedFileFunction realRemoveUsersFromEncryptedFile = NULL;
SaferRecordEventLogEntryFunction realSaferRecordEventLogEntry = NULL;
SaferiIsExecutableFileTypeFunction realSaferiIsExecutableFileType = NULL;
SetFileSecurityAFunction realSetFileSecurityA = NULL;
SetFileSecurityWFunction realSetFileSecurityW = NULL;
SetSecurityDescriptorDaclFunction realSetSecurityDescriptorDacl = NULL;

ReplacementLibrary *library = NULL;
LogSender *logSender = NULL;
Mutex *createProcessMutex = NULL;
Event *childProcessCreatedEvent = NULL;
Event *holodeckAttachedToChildProcessEvent = NULL;
MemoryMappedFile *childProcessDataFile = NULL;
