//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ServicesExeReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement functions injected into services.exe
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 Dec 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ServicesExeReplacementLibrary.h"

//*************************************************************************
// Method:		ReplacementLibraryAttach
// Description: Called when HEAT is attaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryAttach()
{
	ReplacementLibrary::DisableInterception();
	
	if ((kernel32library == NULL) || (advapi32library == NULL))
	{
		advapi32library = new ReplacementLibrary("advapi32.dll");
		kernel32library = new ReplacementLibrary("kernel32.dll");

		createProcessMutex = new Mutex();
		childProcessCreatedEvent = new Event();
		holodeckAttachedToChildProcessEvent = new Event();
		childProcessDataFile = new MemoryMappedFile;

		DWORD pid = GetCurrentProcessId();
		char pidString[12];
		_ultoa(pid, pidString, 10);

		SiString childProcessDataFileName = CHILD_PROC_MAP_FILE_PREFIX;
		childProcessDataFileName += pidString;

		SiString childProcessCreatedEventName = CHILD_PROCESS_EVENT_PREFIX;
		childProcessCreatedEventName += pidString;

		SiString holodeckAttachedToChildProcessEventName = HOLODECK_ATTACHED_PREFIX;
		holodeckAttachedToChildProcessEventName += pidString;

		createProcessMutex->Create();
		childProcessCreatedEvent->Create(childProcessCreatedEventName);
		holodeckAttachedToChildProcessEvent->Create(holodeckAttachedToChildProcessEventName);
		childProcessDataFile->Create(childProcessDataFileName, 256);
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		ReplacementLibraryDetach
// Description: Called when HEAT is detaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryDetach()
{
	ReplacementLibrary::DisableInterception();
	// put all detach code in here

	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		NotifyHolodeckOfCreatedProcess
// Description: Called to tell holodeck that a new process has been created
//	as a child process
// Parameters: 
//	creationFlags - the flags set to create the process
//	pid - the process id of the new process
// Return Value: None
//*************************************************************************
void NotifyHolodeckOfCreatedProcess(DWORD creationFlags, DWORD pid)
{
	// tell holodeck the pid
	char pidString[12];
	_ultoa(pid, pidString, 10);

	SiString fileString = NEW_PROCESS_FLAG;
	fileString += " ";
	fileString += pidString;
	fileString += " ";
	// write the resume app flag
	fileString += ((creationFlags & CREATE_SUSPENDED) ? "0" : "1");
	childProcessDataFile->Write(fileString, (unsigned int)(strlen(fileString) + 1));

	// tell holodeck the process was created
	childProcessCreatedEvent->Signal();

	// wait for holodeck to be done processing
	holodeckAttachedToChildProcessEvent->Acquire();
}

//*************************************************************************
// Method:		DllMain
// Description: Entry point to this dll
// Parameters: See MSDN DllMain function parameters
// Return Value: See MSDN DllMain return value
//*************************************************************************
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	ReplacementLibrary::DisableInterception();
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		ReplacementLibraryDetach();

		if (kernel32library)
		{
			delete kernel32library;
			kernel32library = NULL;
		}

		if (advapi32library)
		{
			delete advapi32library;
			advapi32library = NULL;
		}

		return TRUE; // Don't re-enable intercepts, as we are detaching
	default:
		break;
	}
	ReplacementLibrary::EnableInterception();

	return TRUE;
}

//*************************************************************************
// Method:		CreateProcessAReplacement
// Description: See MSDN CreateProcess function
// Parameters:	See MSDN CreateProcess parameters
// Return Value: See MSDN CreateProcess return value
//*************************************************************************
BOOL WINAPI CreateProcessAReplacement(LPCSTR lpApplicationName, LPSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessA";

	if (realCreateProcessA == NULL)
		realCreateProcessA = (CreateProcessAFunction)kernel32library->GetOriginalFunction(functionName);

	if (realCreateProcessA != NULL)
	{
		DWORD returnValue = realCreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes,
			lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment,
			lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		CreateProcessWReplacement
// Description: See MSDN CreateProcess function
// Parameters:	See MSDN CreateProcess parameters
// Return Value: See MSDN CreateProcess return value
//*************************************************************************
BOOL WINAPI CreateProcessWReplacement(LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessW";

	if (realCreateProcessW == NULL)
		realCreateProcessW = (CreateProcessWFunction)kernel32library->GetOriginalFunction(functionName);

	if (realCreateProcessW != NULL)
	{
		DWORD returnValue = realCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes,
			lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment,
			lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		CreateProcessAsUserAReplacement
// Description: See MSDN CreateProcessAsUser function
// Parameters:	See MSDN CreateProcessAsUser parameters
// Return Value: See MSDN CreateProcessAsUser return value
//*************************************************************************
BOOL WINAPI CreateProcessAsUserAReplacement(HANDLE hToken, LPCSTR lpApplicationName, LPSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessAsUserA";

	if (realCreateProcessAsUserA == NULL)
		realCreateProcessAsUserA = (CreateProcessAsUserAFunction)advapi32library->GetOriginalFunction(functionName);

	if (realCreateProcessAsUserA != NULL)
	{
		DWORD returnValue = realCreateProcessAsUserA(hToken, lpApplicationName, lpCommandLine,
			lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED,
			lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		CreateProcessAsUserWReplacement
// Description: See MSDN CreateProcessAsUser function
// Parameters:	See MSDN CreateProcessAsUser parameters
// Return Value: See MSDN CreateProcessAsUser return value
//*************************************************************************
BOOL WINAPI CreateProcessAsUserWReplacement(HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessAsUserW";

	if (realCreateProcessAsUserW == NULL)
		realCreateProcessAsUserW = (CreateProcessAsUserWFunction)advapi32library->GetOriginalFunction(functionName);

	if (realCreateProcessAsUserW != NULL)
	{
		DWORD returnValue = realCreateProcessAsUserW(hToken, lpApplicationName, lpCommandLine,
			lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED,
			lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		CreateProcessWithLogonWReplacement
// Description: See MSDN CreateProcessWithLogonW function
// Parameters:	See MSDN CreateProcessWithLogonW parameters
// Return Value: See MSDN CreateProcessWithLogonW return value
//*************************************************************************
BOOL WINAPI CreateProcessWithLogonWReplacement(LPCWSTR lpUsername, LPCWSTR lpDomain, 
	LPCWSTR lpPassword, DWORD dwLogonFlags, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
	DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, 
	LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessWithLogonW";

	if (realCreateProcessWithLogonW == NULL)
		realCreateProcessWithLogonW = (CreateProcessWithLogonWFunction)advapi32library->GetOriginalFunction(functionName);

	if (realCreateProcessWithLogonW != NULL)
	{
		DWORD returnValue = realCreateProcessWithLogonW(lpUsername, lpDomain, lpPassword,
			dwLogonFlags, lpApplicationName, lpCommandLine, dwCreationFlags | CREATE_SUSPENDED,
			lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		CreateProcessWithTokenWReplacement
// Description: See MSDN CreateProcessWithTokenWReplacement function
// Parameters:	See MSDN CreateProcessWithTokenWReplacement parameters
// Return Value: See MSDN CreateProcessWithTokenWReplacement return value
//*************************************************************************
BOOL WINAPI CreateProcessWithTokenWReplacement(HANDLE hToken, DWORD dwLogonFlags,
	LPCWSTR lpApplicationName, LPWSTR lpCommandLine, DWORD dwCreationFlags, LPVOID lpEnvironment, 
	LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();
	char *functionName = "CreateProcessWithTokenW";

	if (realCreateProcessWithTokenW == NULL)
		realCreateProcessWithTokenW = (CreateProcessWithTokenWFunction)advapi32library->GetOriginalFunction(functionName);

	if (realCreateProcessWithTokenW != NULL)
	{
		DWORD returnValue = realCreateProcessWithTokenW(hToken, dwLogonFlags, lpApplicationName,
			lpCommandLine, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, 
			lpStartupInfo, lpProcessInformation);

		NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}