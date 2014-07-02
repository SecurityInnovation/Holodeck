//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ServicesExeReplacementLibrary.h
//
// DESCRIPTION: Contains replacement functions injected into services.exe
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 Dec 2003		 B. Shirey	 Created
//*************************************************************************
#pragma once

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "ReplacementLibrary.h"

using namespace Replacement;

typedef WINBASEAPI BOOL (WINAPI *CreateProcessAFunction)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);
typedef WINBASEAPI BOOL (WINAPI *CreateProcessWFunction)(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessAsUserAFunction)(HANDLE, LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessAsUserWFunction)(HANDLE, LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessWithLogonWFunction)(LPCWSTR, LPCWSTR, LPCWSTR, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
typedef WINADVAPI BOOL (WINAPI *CreateProcessWithTokenWFunction)(HANDLE, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);

CreateProcessAFunction realCreateProcessA = NULL;
CreateProcessWFunction realCreateProcessW = NULL;
CreateProcessAsUserAFunction realCreateProcessAsUserA = NULL;
CreateProcessAsUserWFunction realCreateProcessAsUserW = NULL;
CreateProcessWithLogonWFunction realCreateProcessWithLogonW = NULL;
CreateProcessWithTokenWFunction realCreateProcessWithTokenW = NULL;

ReplacementLibrary *kernel32library = NULL;
ReplacementLibrary *advapi32library = NULL;
Mutex *createProcessMutex = NULL;
Event *childProcessCreatedEvent = NULL;
Event *holodeckAttachedToChildProcessEvent = NULL;
MemoryMappedFile *childProcessDataFile = NULL;