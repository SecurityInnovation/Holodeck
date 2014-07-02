//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		AdvapiReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for advapi32.dll
//
//=========================================================================
// Modification History
//
// Generated 05/04/2004 04:51:31 PM
//*************************************************************************
#include "advapireplacementlibrary.h"

//*************************************************************************
// Method:		ReplacementLibraryAttach
// Description: Called when HEAT is attaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryAttach()
{
	ReplacementLibrary::DisableInterception();
	if (library == NULL)
	{
		library = new ReplacementLibrary("advapi32.dll");
		logSender = &commonLogSender;

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

		if (!logSender->GetIsSendLogThreadRunning())
			logSender->StartSendLogThread();
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
	if (library != NULL)
	{
		if ((logSender != NULL) && (logSender->GetIsSendLogThreadRunning()))
		{
			logSender->StopSendLogThread();
			logSender = NULL;
		}
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		DllMain
// Description: Entry point to this dll
// Parameters: See MSDN DllMain function parameters
// Return Value: See MSDN DllMain return value
//*************************************************************************
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	ReplacementLibrary::DisableInterception();
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		ReplacementLibraryDetach();
		delete library;
		library = NULL;
		return TRUE; // Don't re-enable intercepts, as we are detaching
	default:
		break;
	}
	ReplacementLibrary::EnableInterception();

	return TRUE;
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
// START OF ORIGINAL FUNCTION CALLER FUNCTIONS
// For all functions in this section the following applies
// Description:	Calls the original function for a replacement function
// Parameters:
//	numParams - the number of parameters in the params array
//	params - the parameters to pass to the original function
//	retVal - the return value from the original function
//	errCode - the error code from the original function
// Return Value: true if the correct number of parameters were passed in,
//	false otherwise
//*************************************************************************
//*************************************************************************
// Method: RegCloseKeyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegCloseKeyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	*retValue = realRegCloseKey(*((HKEY *)params[0]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegConnectRegistryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegConnectRegistryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegConnectRegistryA(*((SiString *)params[0]), *((HKEY *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegConnectRegistryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegConnectRegistryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegConnectRegistryW(*((SiString *)params[0]), *((HKEY *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegCreateKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegCreateKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegCreateKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegCreateKeyExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegCreateKeyExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	*retValue = realRegCreateKeyExA(*((HKEY *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]),
		*((SiString *)params[3]), *((DWORD *)params[4]), *((REGSAM *)params[5]), *((LPSECURITY_ATTRIBUTES *)params[6]),
		*((PHKEY *)params[7]), *((LPDWORD *)params[8]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegCreateKeyExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegCreateKeyExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	*retValue = realRegCreateKeyExW(*((HKEY *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]),
		*((SiString *)params[3]), *((DWORD *)params[4]), *((DWORD *)params[5]), *((LPSECURITY_ATTRIBUTES *)params[6]),
		*((PHKEY *)params[7]), *((LPDWORD *)params[8]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegCreateKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegCreateKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegCreateKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegDeleteKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegDeleteKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegDeleteKeyA(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegDeleteKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegDeleteKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegDeleteKeyW(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegDeleteValueACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegDeleteValueACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegDeleteValueA(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegDeleteValueWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegDeleteValueWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegDeleteValueW(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegDisablePredefinedCacheCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegDisablePredefinedCacheCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	*retValue = realRegDisablePredefinedCache();
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegEnumKeyA(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPSTR *)params[2]), *((DWORD *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumKeyExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumKeyExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	*retValue = realRegEnumKeyExA(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPSTR *)params[2]), 
		*((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((SiString *)params[5]), *((LPDWORD *)params[6]),
		*((PFILETIME *)params[7]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumKeyExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumKeyExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	*retValue = realRegEnumKeyExW(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPWSTR *)params[2]), 
		*((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((SiString *)params[5]), *((LPDWORD *)params[6]),
		*((PFILETIME *)params[7]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegEnumKeyW(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPWSTR *)params[2]), *((DWORD *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumValueACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumValueACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	*retValue = realRegEnumValueA(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPSTR *)params[2]),
		*((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPDWORD *)params[5]), *((LPBYTE *)params[6]),
		*((LPDWORD *)params[7]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegEnumValueWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegEnumValueWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	*retValue = realRegEnumValueW(*((HKEY *)params[0]), *((DWORD *)params[1]), *((LPWSTR *)params[2]),
		*((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPDWORD *)params[5]), *((LPBYTE *)params[6]),
		*((LPDWORD *)params[7]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegFlushKeyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegFlushKeyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	*retValue = realRegFlushKey(*((HKEY *)params[0]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegGetKeySecurityCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegGetKeySecurityCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegGetKeySecurity(*((HKEY *)params[0]), *((SECURITY_INFORMATION *)params[1]), 
		*((PSECURITY_DESCRIPTOR *)params[2]), *((LPDWORD *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegLoadKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegLoadKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegLoadKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegLoadKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegLoadKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegLoadKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegNotifyChangeKeyValueCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegNotifyChangeKeyValueCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegNotifyChangeKeyValue(*((HKEY *)params[0]), *((BOOL *)params[1]), 
		*((DWORD *)params[2]), *((HANDLE *)params[3]), *((BOOL *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenCurrentUserCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenCurrentUserCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegOpenCurrentUser(*((REGSAM *)params[0]), *((PHKEY *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegOpenKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenKeyExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenKeyExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegOpenKeyExA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((REGSAM *)params[3]), *((PHKEY *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenKeyExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenKeyExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegOpenKeyExW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((REGSAM *)params[3]), *((PHKEY *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegOpenKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), *((PHKEY *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOpenUserClassesRootCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOpenUserClassesRootCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegOpenUserClassesRoot(*((HKEY *)params[0]), *((DWORD *)params[1]), *((REGSAM *)params[2]), *((PHKEY *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegOverridePredefKeyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegOverridePredefKeyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegOverridePredefKey(*((HKEY *)params[0]), *((HKEY *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryInfoKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryInfoKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 12) return false;
	*retValue = realRegQueryInfoKeyA(*((HKEY *)params[0]), *((LPSTR *)params[1]), 
		*((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]),
		*((LPDWORD *)params[5]), *((LPDWORD *)params[6]), *((LPDWORD *)params[7]),
		*((LPDWORD *)params[8]), *((LPDWORD *)params[9]), *((LPDWORD *)params[10]),
		*((PFILETIME *)params[11]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryInfoKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryInfoKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 12) return false;
	*retValue = realRegQueryInfoKeyW(*((HKEY *)params[0]), *((LPWSTR *)params[1]), 
		*((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]),
		*((LPDWORD *)params[5]), *((LPDWORD *)params[6]), *((LPDWORD *)params[7]),
		*((LPDWORD *)params[8]), *((LPDWORD *)params[9]), *((LPDWORD *)params[10]),
		*((PFILETIME *)params[11]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryMultipleValuesACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryMultipleValuesACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegQueryMultipleValuesA(*((HKEY *)params[0]), *((PVALENTA *)params[1]), 
		*((DWORD *)params[2]), *((LPSTR *)params[3]), *((LPDWORD *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryMultipleValuesWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryMultipleValuesWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegQueryMultipleValuesW(*((HKEY *)params[0]), *((PVALENTW *)params[1]), 
		*((DWORD *)params[2]), *((LPWSTR *)params[3]), *((LPDWORD *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryValueACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryValueACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegQueryValueA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPSTR *)params[2]), *((PLONG *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryValueWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryValueWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegQueryValueW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPWSTR *)params[2]), *((PLONG *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryValueExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryValueExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	*retValue = realRegQueryValueExA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPBYTE *)params[4]), 
		*((LPDWORD *)params[5]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegQueryValueExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegQueryValueExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	*retValue = realRegQueryValueExW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPBYTE *)params[4]), 
		*((LPDWORD *)params[5]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegReplaceKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegReplaceKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegReplaceKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((SiString *)params[2]), *((SiString *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegReplaceKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegReplaceKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegReplaceKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((SiString *)params[2]), *((SiString *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegRestoreKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegRestoreKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegRestoreKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegRestoreKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegRestoreKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegRestoreKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSaveKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSaveKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegSaveKeyA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSaveKeyExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSaveKeyExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegSaveKeyExA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]), *((DWORD *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSaveKeyExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSaveKeyExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	*retValue = realRegSaveKeyExW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]), *((DWORD *)params[3]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSaveKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSaveKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegSaveKeyW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSetKeySecurityCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSetKeySecurityCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	*retValue = realRegSetKeySecurity(*((HKEY *)params[0]), *((SECURITY_INFORMATION *)params[1]), 
		*((PSECURITY_DESCRIPTOR *)params[2]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSetValueACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSetValueACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegSetValueA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((SiString *)params[3]), *((DWORD *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSetValueExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSetValueExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	*retValue = realRegSetValueExA(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((DWORD *)params[3]), *((CONST BYTE **)params[4]),
		*((DWORD *)params[5]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSetValueExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSetValueExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	*retValue = realRegSetValueExW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((DWORD *)params[3]), *((CONST BYTE **)params[4]),
		*((DWORD *)params[5]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegSetValueWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegSetValueWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = realRegSetValueW(*((HKEY *)params[0]), *((SiString *)params[1]), 
		*((DWORD *)params[2]), *((SiString *)params[3]), *((DWORD *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegUnLoadKeyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegUnLoadKeyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegUnLoadKeyA(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RegUnLoadKeyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RegUnLoadKeyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	*retValue = realRegUnLoadKeyW(*((HKEY *)params[0]), *((SiString *)params[1]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: BackupEventLogACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool BackupEventLogACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realBackupEventLogA(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: BackupEventLogWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool BackupEventLogWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realBackupEventLogW(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ClearEventLogACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ClearEventLogACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realClearEventLogA(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ClearEventLogWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ClearEventLogWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realClearEventLogW(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CloseEventLogCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CloseEventLogCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realCloseEventLog(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessAsUserACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessAsUserACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 11) return false;
	*retValue = realCreateProcessAsUserA(*((HANDLE *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]), *((LPSECURITY_ATTRIBUTES *)params[4]), *((BOOL *)params[5]), *((DWORD *)params[6]) | CREATE_SUSPENDED, *((LPVOID *)params[7]), *((SiString *)params[8]), *((LPSTARTUPINFOA *)params[9]), *((LPPROCESS_INFORMATION *)params[10]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessAsUserWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessAsUserWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 11) return false;
	*retValue = realCreateProcessAsUserW(*((HANDLE *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]), *((LPSECURITY_ATTRIBUTES *)params[4]), *((BOOL *)params[5]), *((DWORD *)params[6]) | CREATE_SUSPENDED, *((LPVOID *)params[7]), *((SiString *)params[8]), *((LPSTARTUPINFOW *)params[9]), *((LPPROCESS_INFORMATION *)params[10]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessWithLogonWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessWithLogonWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 11) return false;
	*retValue = realCreateProcessWithLogonW(*((SiString *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((DWORD *)params[3]), *((SiString *)params[4]), *((SiString *)params[5]), *((DWORD *)params[6]) | CREATE_SUSPENDED, *((LPVOID *)params[7]), *((SiString *)params[8]), *((LPSTARTUPINFOW *)params[9]), *((LPPROCESS_INFORMATION *)params[10]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessWithTokenWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessWithTokenWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	*retValue = realCreateProcessWithTokenW(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((SiString *)params[2]), *((SiString *)params[3]), *((DWORD *)params[4]) | CREATE_SUSPENDED, *((LPVOID *)params[5]), *((SiString *)params[6]), *((LPSTARTUPINFOW *)params[7]), *((LPPROCESS_INFORMATION *)params[8]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DecryptFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DecryptFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realDecryptFileA(*((SiString *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DecryptFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DecryptFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realDecryptFileW(*((SiString *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: EncryptFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool EncryptFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realEncryptFileA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: EncryptFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool EncryptFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realEncryptFileW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FileEncryptionStatusACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FileEncryptionStatusACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFileEncryptionStatusA(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FileEncryptionStatusWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FileEncryptionStatusWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFileEncryptionStatusW(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileSecurityACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileSecurityACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realGetFileSecurityA(*((SiString *)params[0]), *((SECURITY_INFORMATION *)params[1]), *((PSECURITY_DESCRIPTOR *)params[2]), *((DWORD *)params[3]), *((LPDWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileSecurityWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileSecurityWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realGetFileSecurityW(*((SiString *)params[0]), *((SECURITY_INFORMATION *)params[1]), *((PSECURITY_DESCRIPTOR *)params[2]), *((DWORD *)params[3]), *((LPDWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OpenBackupEventLogACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OpenBackupEventLogACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realOpenBackupEventLogA(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OpenBackupEventLogWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OpenBackupEventLogWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realOpenBackupEventLogW(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OpenEventLogACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OpenEventLogACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realOpenEventLogA(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OpenEventLogWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OpenEventLogWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realOpenEventLogW(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: QueryUsersOnEncryptedFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool QueryUsersOnEncryptedFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realQueryUsersOnEncryptedFile(*((SiString *)params[0]), *((PENCRYPTION_CERTIFICATE_HASH_LIST * *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RemoveUsersFromEncryptedFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RemoveUsersFromEncryptedFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realRemoveUsersFromEncryptedFile(*((SiString *)params[0]), *((PENCRYPTION_CERTIFICATE_HASH_LIST *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SaferRecordEventLogEntryCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SaferRecordEventLogEntryCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realSaferRecordEventLogEntry(*((SAFER_LEVEL_HANDLE *)params[0]), *((SiString *)params[1]), *((LPVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SaferiIsExecutableFileTypeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SaferiIsExecutableFileTypeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSaferiIsExecutableFileType(*((SiString *)params[0]), *((BOOLEAN *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileSecurityACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileSecurityACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realSetFileSecurityA(*((SiString *)params[0]), *((SECURITY_INFORMATION *)params[1]), *((PSECURITY_DESCRIPTOR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileSecurityWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileSecurityWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realSetFileSecurityW(*((SiString *)params[0]), *((SECURITY_INFORMATION *)params[1]), *((PSECURITY_DESCRIPTOR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetSecurityDescriptorDaclCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetSecurityDescriptorDaclCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realSetSecurityDescriptorDacl(*((PSECURITY_DESCRIPTOR *)params[0]), *((BOOL *)params[1]), *((PACL *)params[2]), *((BOOL *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// END OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************

//*************************************************************************
// START OF REPLACEMENT FUNCTIONS
//*************************************************************************
//*************************************************************************
// Method:		RegCloseKeyReplacement
// Description: See MSDN RegCloseKey function
// Parameters:	See MSDN RegCloseKey parameters
// Return Value: See MSDN RegCloseKey return value
//*************************************************************************
LONG APIENTRY RegCloseKeyReplacement(HKEY hKey)
{
	const int numParams = 1;
	char *functionName = "RegCloseKey";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey };
	ParameterType paramTypes[numParams] = { PointerType };

	if (realRegCloseKey == NULL)
		realRegCloseKey = (RegCloseKeyFunction)library->GetOriginalFunction(functionName);

	if (realRegCloseKey != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegCloseKeyCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegConnectRegistryAReplacement
// Description: See MSDN RegConnectRegistry function
// Parameters:	See MSDN RegConnectRegistry parameters
// Return Value: See MSDN RegConnectRegistry return value
//*************************************************************************
LONG APIENTRY RegConnectRegistryAReplacement(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegConnectRegistryA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpMachineName;
	void *params[numParams] = { &str, &hKey, &phkResult };
	ParameterType paramTypes[numParams] = { StringType, PointerType, PointerPointerType };

	if (realRegConnectRegistryA == NULL)
		realRegConnectRegistryA = (RegConnectRegistryAFunction)library->GetOriginalFunction(functionName);

	if (realRegConnectRegistryA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpMachineName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegConnectRegistryACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegConnectRegistryWReplacement
// Description: See MSDN RegConnectRegistry function
// Parameters:	See MSDN RegConnectRegistry parameters
// Return Value: See MSDN RegConnectRegistry return value
//*************************************************************************
LONG APIENTRY RegConnectRegistryWReplacement(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegConnectRegistryW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpMachineName;
	void *params[numParams] = { &str, &hKey, &phkResult };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, PointerPointerType };

	if (realRegConnectRegistryW == NULL)
		realRegConnectRegistryW = (RegConnectRegistryWFunction)library->GetOriginalFunction(functionName);

	if (realRegConnectRegistryW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpMachineName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegConnectRegistryWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegCreateKeyAReplacement
// Description: See MSDN RegCreateKey function
// Parameters:	See MSDN RegCreateKey parameters
// Return Value: See MSDN RegCreateKey return value
//*************************************************************************
LONG APIENTRY RegCreateKeyAReplacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegCreateKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, StringType, PointerPointerType };

	if (realRegCreateKeyA == NULL)
		realRegCreateKeyA = (RegCreateKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegCreateKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegCreateKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegCreateKeyExAReplacement
// Description: See MSDN RegCreateKeyEx function
// Parameters:	See MSDN RegCreateKeyEx parameters
// Return Value: See MSDN RegCreateKeyEx return value
//*************************************************************************
LONG APIENTRY RegCreateKeyExAReplacement(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	const int numParams = 9;
	char *functionName = "RegCreateKeyExA";
	char *categoryName = "REGISTRY";
	SiString str[] = { (char *)lpSubKey, (char *)lpClass };
	void *params[numParams] = { &hKey, &str[0], &Reserved, &str[1], &dwOptions, &samDesired, &lpSecurityAttributes, &phkResult, &lpdwDisposition };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType, StringType, UnsignedLongType, UnsignedLongType, PointerType, PointerPointerType, PointerType };

	if (realRegCreateKeyExA == NULL)
		realRegCreateKeyExA = (RegCreateKeyExAFunction)library->GetOriginalFunction(functionName);

	if (realRegCreateKeyExA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegCreateKeyExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegCreateKeyExWReplacement
// Description: See MSDN RegCreateKeyEx function
// Parameters:	See MSDN RegCreateKeyEx parameters
// Return Value: See MSDN RegCreateKeyEx return value
//*************************************************************************
LONG APIENTRY RegCreateKeyExWReplacement(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	const int numParams = 9;
	char *functionName = "RegCreateKeyExW";
	char *categoryName = "REGISTRY";
	SiString str[] = { (wchar_t *)lpSubKey, (wchar_t *)lpClass };
	void *params[numParams] = { &hKey, &str[0], &Reserved, &str[1], &dwOptions, &samDesired, &lpSecurityAttributes, &phkResult, &lpdwDisposition };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType, WideStringType, UnsignedLongType, UnsignedLongType, PointerType, PointerPointerType, PointerType };

	if (realRegCreateKeyExW == NULL)
		realRegCreateKeyExW = (RegCreateKeyExWFunction)library->GetOriginalFunction(functionName);

	if (realRegCreateKeyExW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegCreateKeyExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegCreateKeyWReplacement
// Description: See MSDN RegCreateKey function
// Parameters:	See MSDN RegCreateKey parameters
// Return Value: See MSDN RegCreateKey return value
//*************************************************************************
LONG APIENTRY RegCreateKeyWReplacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegCreateKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerPointerType };

	if (realRegCreateKeyW == NULL)
		realRegCreateKeyW = (RegCreateKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegCreateKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegCreateKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegDeleteKeyAReplacement
// Description: See MSDN RegDeleteKey function
// Parameters:	See MSDN RegDeleteKey parameters
// Return Value: See MSDN RegDeleteKey return value
//*************************************************************************
LONG APIENTRY RegDeleteKeyAReplacement(HKEY hKey, LPCSTR lpSubKey)
{
	const int numParams = 2;
	char *functionName = "RegDeleteKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, StringType };

	if (realRegDeleteKeyA == NULL)
		realRegDeleteKeyA = (RegDeleteKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegDeleteKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegDeleteKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegDeleteKeyWReplacement
// Description: See MSDN RegDeleteKey function
// Parameters:	See MSDN RegDeleteKey parameters
// Return Value: See MSDN RegDeleteKey return value
//*************************************************************************
LONG APIENTRY RegDeleteKeyWReplacement(HKEY hKey, LPCWSTR lpSubKey)
{
	const int numParams = 2;
	char *functionName = "RegDeleteKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };

	if (realRegDeleteKeyW == NULL)
		realRegDeleteKeyW = (RegDeleteKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegDeleteKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegDeleteKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegDeleteValueAReplacement
// Description: See MSDN RegDeleteValue function
// Parameters:	See MSDN RegDeleteValue parameters
// Return Value: See MSDN RegDeleteValue return value
//*************************************************************************
LONG APIENTRY RegDeleteValueAReplacement(HKEY hKey, LPCSTR lpValueName)
{
	const int numParams = 2;
	char *functionName = "RegDeleteValueA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpValueName;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, StringType };

	if (realRegDeleteValueA == NULL)
		realRegDeleteValueA = (RegDeleteValueAFunction)library->GetOriginalFunction(functionName);

	if (realRegDeleteValueA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegDeleteValueACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegDeleteValueWReplacement
// Description: See MSDN RegDeleteValue function
// Parameters:	See MSDN RegDeleteValue parameters
// Return Value: See MSDN RegDeleteValue return value
//*************************************************************************
LONG APIENTRY RegDeleteValueWReplacement(HKEY hKey, LPCWSTR lpValueName)
{
	const int numParams = 2;
	char *functionName = "RegDeleteValueW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpValueName;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };

	if (realRegDeleteValueW == NULL)
		realRegDeleteValueW = (RegDeleteValueWFunction)library->GetOriginalFunction(functionName);

	if (realRegDeleteValueW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegDeleteValueWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegDisablePredefinedCacheReplacement
// Description: See MSDN RegDisablePredefinedCache function
// Parameters:	See MSDN RegDisablePredefinedCache parameters
// Return Value: See MSDN RegDisablePredefinedCache return value
//*************************************************************************
LONG APIENTRY RegDisablePredefinedCacheReplacement()
{
	const int numParams = 0;
	char *functionName = "RegDisablePredefinedCache";
	char *categoryName = "REGISTRY";
	void **params = NULL;
	ParameterType *paramTypes = NULL;

	if (realRegDisablePredefinedCache == NULL)
		realRegDisablePredefinedCache = (RegDisablePredefinedCacheFunction)library->GetOriginalFunction(functionName);

	if (realRegDisablePredefinedCache != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegDisablePredefinedCacheCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumKeyAReplacement
// Description: See MSDN RegEnumKey function
// Parameters:	See MSDN RegEnumKey parameters
// Return Value: See MSDN RegEnumKey return value
//*************************************************************************
LONG APIENTRY RegEnumKeyAReplacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD cbName)
{
	const int numParams = 4;
	char *functionName = "RegEnumKeyA";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &dwIndex, &lpName, &cbName };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutStringType, UnsignedLongType };

	if (realRegEnumKeyA == NULL)
		realRegEnumKeyA = (RegEnumKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumKeyExAReplacement
// Description: See MSDN RegEnumKeyEx function
// Parameters:	See MSDN RegEnumKeyEx parameters
// Return Value: See MSDN RegEnumKeyEx return value
//*************************************************************************
LONG APIENTRY RegEnumKeyExAReplacement(HKEY hKey, DWORD dwIndex, LPSTR lpName, LPDWORD lpcbName, LPDWORD lpReserved, LPSTR lpClass, LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	const int numParams = 8;
	char *functionName = "RegEnumKeyExA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpClass ;
	void *params[numParams] = { &hKey, &dwIndex, &lpName, &lpcbName, &lpReserved, &str, &lpcbClass, &lpftLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutStringType, PointerType, PointerType, StringType, PointerType, PointerType };

	if (realRegEnumKeyExA == NULL)
		realRegEnumKeyExA = (RegEnumKeyExAFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumKeyExA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumKeyExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumKeyExWReplacement
// Description: See MSDN RegEnumKeyEx function
// Parameters:	See MSDN RegEnumKeyEx parameters
// Return Value: See MSDN RegEnumKeyEx return value
//*************************************************************************
LONG APIENTRY RegEnumKeyExWReplacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcbName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	const int numParams = 8;
	char *functionName = "RegEnumKeyExW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpClass;
	void *params[numParams] = { &hKey, &dwIndex, &lpName, &lpcbName, &lpReserved, &str, &lpcbClass, &lpftLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutWideStringType, PointerType, PointerType, WideStringType, PointerType, PointerType };

	if (realRegEnumKeyExW == NULL)
		realRegEnumKeyExW = (RegEnumKeyExWFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumKeyExW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumKeyExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumKeyWReplacement
// Description: See MSDN RegEnumKey function
// Parameters:	See MSDN RegEnumKey parameters
// Return Value: See MSDN RegEnumKey return value
//*************************************************************************
LONG APIENTRY RegEnumKeyWReplacement(HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cbName)
{
	const int numParams = 4;
	char *functionName = "RegEnumKeyW";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &dwIndex, &lpName, &cbName };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutWideStringType, UnsignedLongType };

	if (realRegEnumKeyW == NULL)
		realRegEnumKeyW = (RegEnumKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumValueAReplacement
// Description: See MSDN RegEnumValue function
// Parameters:	See MSDN RegEnumValue parameters
// Return Value: See MSDN RegEnumValue return value
//*************************************************************************
LONG APIENTRY RegEnumValueAReplacement(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcbValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	const int numParams = 8;
	char *functionName = "RegEnumValueA";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &dwIndex, &lpValueName, &lpcbValueName, &lpReserved, &lpType, &lpData, &lpcbData };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutStringType, PointerType, PointerType, PointerType, PointerType, PointerType };

	if (realRegEnumValueA == NULL)
		realRegEnumValueA = (RegEnumValueAFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumValueA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumValueACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegEnumValueWReplacement
// Description: See MSDN RegEnumValue function
// Parameters:	See MSDN RegEnumValue parameters
// Return Value: See MSDN RegEnumValue return value
//*************************************************************************
LONG APIENTRY RegEnumValueWReplacement(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcbValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	const int numParams = 8;
	char *functionName = "RegEnumValueW";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &dwIndex, &lpValueName, &lpcbValueName, &lpReserved, &lpType, &lpData, &lpcbData };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, OutWideStringType, PointerType, PointerType, PointerType, PointerType, PointerType };

	if (realRegEnumValueW == NULL)
		realRegEnumValueW = (RegEnumValueWFunction)library->GetOriginalFunction(functionName);

	if (realRegEnumValueW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegEnumValueWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegFlushKeyReplacement
// Description: See MSDN RegFlushKey function
// Parameters:	See MSDN RegFlushKey parameters
// Return Value: See MSDN RegFlushKey return value
//*************************************************************************
LONG APIENTRY RegFlushKeyReplacement(HKEY hKey)
{
	const int numParams = 1;
	char *functionName = "RegFlushKey";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey };
	ParameterType paramTypes[numParams] = { PointerType };

	if (realRegFlushKey == NULL)
		realRegFlushKey = (RegFlushKeyFunction)library->GetOriginalFunction(functionName);

	if (realRegFlushKey != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegFlushKeyCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegGetKeySecurityReplacement
// Description: See MSDN RegGetKeySecurity function
// Parameters:	See MSDN RegGetKeySecurity parameters
// Return Value: See MSDN RegGetKeySecurity return value
//*************************************************************************
LONG APIENTRY RegGetKeySecurityReplacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	const int numParams = 4;
	char *functionName = "RegGetKeySecurity";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &SecurityInformation, &pSecurityDescriptor, &lpcbSecurityDescriptor };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, PointerType };

	if (realRegGetKeySecurity == NULL)
		realRegGetKeySecurity = (RegGetKeySecurityFunction)library->GetOriginalFunction(functionName);

	if (realRegGetKeySecurity != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegGetKeySecurityCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegLoadKeyAReplacement
// Description: See MSDN RegLoadKey function
// Parameters:	See MSDN RegLoadKey parameters
// Return Value: See MSDN RegLoadKey return value
//*************************************************************************
LONG APIENTRY RegLoadKeyAReplacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpFile)
{
	const int numParams = 3;
	char *functionName = "RegLoadKeyA";
	char *categoryName = "REGISTRY";
	SiString str[] = { (char *)lpSubKey, (char *)lpFile };
	void *params[numParams] = { &hKey, &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { PointerType, StringType, StringType };

	if (realRegLoadKeyA == NULL)
		realRegLoadKeyA = (RegLoadKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegLoadKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegLoadKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegLoadKeyWReplacement
// Description: See MSDN RegLoadKey function
// Parameters:	See MSDN RegLoadKey parameters
// Return Value: See MSDN RegLoadKey return value
//*************************************************************************
LONG APIENTRY RegLoadKeyWReplacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpFile)
{
	const int numParams = 3;
	char *functionName = "RegLoadKeyW";
	char *categoryName = "REGISTRY";
	SiString str[] = { (wchar_t *)lpSubKey, (wchar_t *)lpFile };
	void *params[numParams] = { &hKey, &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, WideStringType };

	if (realRegLoadKeyW == NULL)
		realRegLoadKeyW = (RegLoadKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegLoadKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegLoadKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegNotifyChangeKeyValueReplacement
// Description: See MSDN RegNotifyChangeKeyValue function
// Parameters:	See MSDN RegNotifyChangeKeyValue parameters
// Return Value: See MSDN RegNotifyChangeKeyValue return value
//*************************************************************************
LONG APIENTRY RegNotifyChangeKeyValueReplacement(HKEY hKey, BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronus)
{
	const int numParams = 5;
	char *functionName = "RegNotifyChangeKeyValue";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &bWatchSubtree, &dwNotifyFilter, &hEvent, &fAsynchronus };
	ParameterType paramTypes[numParams] = { PointerType, BooleanType, UnsignedLongType, PointerType, BooleanType };

	if (realRegNotifyChangeKeyValue == NULL)
		realRegNotifyChangeKeyValue = (RegNotifyChangeKeyValueFunction)library->GetOriginalFunction(functionName);

	if (realRegNotifyChangeKeyValue != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegNotifyChangeKeyValueCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenCurrentUserReplacement
// Description: See MSDN RegOpenCurrentUser function
// Parameters:	See MSDN RegOpenCurrentUser parameters
// Return Value: See MSDN RegOpenCurrentUser return value
//*************************************************************************
LONG APIENTRY RegOpenCurrentUserReplacement(REGSAM samDesired, PHKEY phkResult)
{
	const int numParams = 2;
	char *functionName = "RegOpenCurrentUser";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &samDesired, &phkResult };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerPointerType };

	if (realRegOpenCurrentUser == NULL)
		realRegOpenCurrentUser = (RegOpenCurrentUserFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenCurrentUser != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegOpenCurrentUserCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateHandleMapping(*phkResult, HKEY_CURRENT_USER);
		}
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenKeyAReplacement
// Description: See MSDN RegOpenKey function
// Parameters:	See MSDN RegOpenKey parameters
// Return Value: See MSDN RegOpenKey return value
//*************************************************************************
LONG APIENTRY RegOpenKeyAReplacement(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegOpenKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, StringType, PointerPointerType };

	if (realRegOpenKeyA == NULL)
		realRegOpenKeyA = (RegOpenKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegOpenKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenKeyExAReplacement
// Description: See MSDN RegOpenKeyEx function
// Parameters:	See MSDN RegOpenKeyEx parameters
// Return Value: See MSDN RegOpenKeyEx return value
//*************************************************************************
LONG APIENTRY RegOpenKeyExAReplacement(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	const int numParams = 5;
	char *functionName = "RegOpenKeyExA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &ulOptions, &samDesired, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType, UnsignedLongType, PointerPointerType };

	if (realRegOpenKeyExA == NULL)
		realRegOpenKeyExA = (RegOpenKeyExAFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenKeyExA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegOpenKeyExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenKeyExWReplacement
// Description: See MSDN RegOpenKeyEx function
// Parameters:	See MSDN RegOpenKeyEx parameters
// Return Value: See MSDN RegOpenKeyEx return value
//*************************************************************************
LONG APIENTRY RegOpenKeyExWReplacement(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	const int numParams = 5;
	char *functionName = "RegOpenKeyExW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &ulOptions, &samDesired, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType, UnsignedLongType, PointerPointerType };

	if (realRegOpenKeyExW == NULL)
		realRegOpenKeyExW = (RegOpenKeyExWFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenKeyExW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegOpenKeyExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenKeyWReplacement
// Description: See MSDN RegOpenKey function
// Parameters:	See MSDN RegOpenKey parameters
// Return Value: See MSDN RegOpenKey return value
//*************************************************************************
LONG APIENTRY RegOpenKeyWReplacement(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	const int numParams = 3;
	char *functionName = "RegOpenKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerPointerType };

	if (realRegOpenKeyW == NULL)
		realRegOpenKeyW = (RegOpenKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegOpenKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateResourceMapping(*phkResult, resourcePath);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOpenUserClassesRootReplacement
// Description: See MSDN RegOpenUserClassesRoot function
// Parameters:	See MSDN RegOpenUserClassesRoot parameters
// Return Value: See MSDN RegOpenUserClassesRoot return value
//*************************************************************************
LONG APIENTRY RegOpenUserClassesRootReplacement(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
{
	const int numParams = 4;
	char *functionName = "RegOpenUserClassesRoot";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hToken, &dwOptions, &samDesired, &phkResult };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, PointerPointerType };

	if (realRegOpenUserClassesRoot == NULL)
		realRegOpenUserClassesRoot = (RegOpenUserClassesRootFunction)library->GetOriginalFunction(functionName);

	if (realRegOpenUserClassesRoot != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegOpenUserClassesRootCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		if (phkResult != NULL)
		{
			//map new path to the resulting handle
			library->CreateHandleMapping(*phkResult, HKEY_CLASSES_ROOT);
		}
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegOverridePredefKeyReplacement
// Description: See MSDN RegOverridePredefKey function
// Parameters:	See MSDN RegOverridePredefKey parameters
// Return Value: See MSDN RegOverridePredefKey return value
//*************************************************************************
LONG APIENTRY RegOverridePredefKeyReplacement(HKEY hKey, HKEY hNewHKey)
{
	const int numParams = 2;
	char *functionName = "RegOverridePredefKey";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &hNewHKey };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };

	if (realRegOverridePredefKey == NULL)
		realRegOverridePredefKey = (RegOverridePredefKeyFunction)library->GetOriginalFunction(functionName);

	if (realRegOverridePredefKey != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegOverridePredefKeyCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		library->CreateHandleMapping(hNewHKey, hKey);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryInfoKeyAReplacement
// Description: See MSDN RegQueryInfoKey function
// Parameters:	See MSDN RegQueryInfoKey parameters
// Return Value: See MSDN RegQueryInfoKey return value
//*************************************************************************
LONG APIENTRY RegQueryInfoKeyAReplacement(HKEY hKey, LPSTR lpClass, LPDWORD lpcbClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	const int numParams = 12;
	char *functionName = "RegQueryInfoKeyA";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &lpClass, &lpcbClass, &lpReserved, &lpcSubKeys, &lpcbMaxSubKeyLen, &lpcbMaxClassLen, &lpcValues, &lpcbMaxValueNameLen, &lpcbMaxValueLen, &lpcbSecurityDescriptor, &lpftLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, OutStringType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType };

	if (realRegQueryInfoKeyA == NULL)
		realRegQueryInfoKeyA = (RegQueryInfoKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryInfoKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegQueryInfoKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryInfoKeyWReplacement
// Description: See MSDN RegQueryInfoKey function
// Parameters:	See MSDN RegQueryInfoKey parameters
// Return Value: See MSDN RegQueryInfoKey return value
//*************************************************************************
LONG APIENTRY RegQueryInfoKeyWReplacement(HKEY hKey, LPWSTR lpClass, LPDWORD lpcbClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	const int numParams = 12;
	char *functionName = "RegQueryInfoKeyW";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &lpClass, &lpcbClass, &lpReserved, &lpcSubKeys, &lpcbMaxSubKeyLen, &lpcbMaxClassLen, &lpcValues, &lpcbMaxValueNameLen, &lpcbMaxValueLen, &lpcbSecurityDescriptor, &lpftLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, OutWideStringType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType };

	if (realRegQueryInfoKeyW == NULL)
		realRegQueryInfoKeyW = (RegQueryInfoKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryInfoKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegQueryInfoKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryMultipleValuesAReplacement
// Description: See MSDN RegQueryMultipleValues function
// Parameters:	See MSDN RegQueryMultipleValues parameters
// Return Value: See MSDN RegQueryMultipleValues return value
//*************************************************************************
LONG APIENTRY RegQueryMultipleValuesAReplacement(HKEY hKey, PVALENTA val_list, DWORD num_vals, LPSTR lpValueBuf, LPDWORD ldwTotsize)
{
	const int numParams = 5;
	char *functionName = "RegQueryMultipleValuesA";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &val_list, &num_vals, &lpValueBuf, &ldwTotsize };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, OutStringType, PointerType };

	if (realRegQueryMultipleValuesA == NULL)
		realRegQueryMultipleValuesA = (RegQueryMultipleValuesAFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryMultipleValuesA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegQueryMultipleValuesACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryMultipleValuesWReplacement
// Description: See MSDN RegQueryMultipleValues function
// Parameters:	See MSDN RegQueryMultipleValues parameters
// Return Value: See MSDN RegQueryMultipleValues return value
//*************************************************************************
LONG APIENTRY RegQueryMultipleValuesWReplacement(HKEY hKey, PVALENTW val_list, DWORD num_vals, LPWSTR lpValueBuf, LPDWORD ldwTotsize)
{
	const int numParams = 5;
	char *functionName = "RegQueryMultipleValuesW";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &val_list, &num_vals, &lpValueBuf, &ldwTotsize };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, OutWideStringType, PointerType };

	if (realRegQueryMultipleValuesW == NULL)
		realRegQueryMultipleValuesW = (RegQueryMultipleValuesWFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryMultipleValuesW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegQueryMultipleValuesWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryValueAReplacement
// Description: See MSDN RegQueryValue function
// Parameters:	See MSDN RegQueryValue parameters
// Return Value: See MSDN RegQueryValue return value
//*************************************************************************
LONG APIENTRY RegQueryValueAReplacement(HKEY hKey, LPCSTR lpSubKey, LPSTR lpValue, PLONG lpcbValue)
{
	const int numParams = 4;
	char *functionName = "RegQueryValueA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &lpValue, &lpcbValue };
	ParameterType paramTypes[numParams] = { PointerType, StringType, OutStringType, PointerType };

	if (realRegQueryValueA == NULL)
		realRegQueryValueA = (RegQueryValueAFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryValueA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegQueryValueACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryValueWReplacement
// Description: See MSDN RegQueryValue function
// Parameters:	See MSDN RegQueryValue parameters
// Return Value: See MSDN RegQueryValue return value
//*************************************************************************
LONG APIENTRY RegQueryValueWReplacement(HKEY hKey, LPCWSTR lpSubKey, LPSTR lpValue, PLONG lpcbValue)
{
	const int numParams = 4;
	char *functionName = "RegQueryValueW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str, &lpValue, &lpcbValue };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, OutWideStringType, PointerType };

	if (realRegQueryValueW == NULL)
		realRegQueryValueW = (RegQueryValueWFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryValueW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegQueryValueWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryValueExAReplacement
// Description: See MSDN RegQueryValueEx function
// Parameters:	See MSDN RegQueryValueEx parameters
// Return Value: See MSDN RegQueryValueEx return value
//*************************************************************************
LONG APIENTRY RegQueryValueExAReplacement(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	const int numParams = 6;
	char *functionName = "RegQueryValueExA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpValueName;
	void *params[numParams] = { &hKey, &str, &lpReserved, &lpType, &lpData, &lpcbData };
	ParameterType paramTypes[numParams] = { PointerType, StringType, PointerType, PointerType, PointerType, PointerType };

	if (realRegQueryValueExA == NULL)
		realRegQueryValueExA = (RegQueryValueExAFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryValueExA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegQueryValueExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegQueryValueExWReplacement
// Description: See MSDN RegQueryValueEx function
// Parameters:	See MSDN RegQueryValueEx parameters
// Return Value: See MSDN RegQueryValueEx return value
//*************************************************************************
LONG APIENTRY RegQueryValueExWReplacement(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	const int numParams = 6;
	char *functionName = "RegQueryValueExW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpValueName;
	void *params[numParams] = { &hKey, &str, &lpReserved, &lpType, &lpData, &lpcbData };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerType, PointerType, PointerType, PointerType };

	if (realRegQueryValueExW == NULL)
		realRegQueryValueExW = (RegQueryValueExWFunction)library->GetOriginalFunction(functionName);

	if (realRegQueryValueExW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegQueryValueExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegReplaceKeyAReplacement
// Description: See MSDN RegReplaceKey function
// Parameters:	See MSDN RegReplaceKey parameters
// Return Value: See MSDN RegReplaceKey return value
//*************************************************************************
LONG APIENTRY RegReplaceKeyAReplacement(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpNewFile, LPCSTR lpOldFile)
{
	const int numParams = 4;
	char *functionName = "RegReplaceKeyA";
	char *categoryName = "REGISTRY";
	SiString str[] = { (char *)lpSubKey, (char *)lpNewFile, (char *)lpOldFile };
	void *params[numParams] = { &hKey, &str[0], &str[1], &str[2] };
	ParameterType paramTypes[numParams] = { PointerType, StringType, StringType, StringType };

	if (realRegReplaceKeyA == NULL)
		realRegReplaceKeyA = (RegReplaceKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegReplaceKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegReplaceKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegReplaceKeyWReplacement
// Description: See MSDN RegReplaceKey function
// Parameters:	See MSDN RegReplaceKey parameters
// Return Value: See MSDN RegReplaceKey return value
//*************************************************************************
LONG APIENTRY RegReplaceKeyWReplacement(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpNewFile, LPCWSTR lpOldFile)
{
	const int numParams = 4;
	char *functionName = "RegReplaceKeyW";
	char *categoryName = "REGISTRY";
	SiString str[] = { (wchar_t *)lpSubKey, (wchar_t *)lpNewFile, (wchar_t *)lpOldFile };
	void *params[numParams] = { &hKey, &str[0], &str[1], &str[2] };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, WideStringType, WideStringType };

	if (realRegReplaceKeyW == NULL)
		realRegReplaceKeyW = (RegReplaceKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegReplaceKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegReplaceKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegRestoreKeyAReplacement
// Description: See MSDN RegRestoreKey function
// Parameters:	See MSDN RegRestoreKey parameters
// Return Value: See MSDN RegRestoreKey return value
//*************************************************************************
LONG APIENTRY RegRestoreKeyAReplacement(HKEY hKey, LPCSTR lpFile, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "RegRestoreKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpFile;
	void *params[numParams] = { &hKey, &str, &dwFlags };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType };

	if (realRegRestoreKeyA == NULL)
		realRegRestoreKeyA = (RegRestoreKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegRestoreKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegRestoreKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegRestoreKeyWReplacement
// Description: See MSDN RegRestoreKey function
// Parameters:	See MSDN RegRestoreKey parameters
// Return Value: See MSDN RegRestoreKey return value
//*************************************************************************
LONG APIENTRY RegRestoreKeyWReplacement(HKEY hKey, LPCWSTR lpFile, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "RegRestoreKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpFile;
	void *params[numParams] = { &hKey, &str, &dwFlags };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType };

	if (realRegRestoreKeyW == NULL)
		realRegRestoreKeyW = (RegRestoreKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegRestoreKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegRestoreKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSaveKeyAReplacement
// Description: See MSDN RegSaveKey function
// Parameters:	See MSDN RegSaveKey parameters
// Return Value: See MSDN RegSaveKey return value
//*************************************************************************
LONG APIENTRY RegSaveKeyAReplacement(HKEY hKey, LPCSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "RegSaveKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpFile;
	void *params[numParams] = { &hKey, &str, &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { PointerType, StringType, PointerType };

	if (realRegSaveKeyA == NULL)
		realRegSaveKeyA = (RegSaveKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegSaveKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegSaveKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSaveKeyExAReplacement
// Description: See MSDN RegSaveKeyEx function
// Parameters:	See MSDN RegSaveKeyEx parameters
// Return Value: See MSDN RegSaveKeyEx return value
//*************************************************************************
LONG APIENTRY RegSaveKeyExAReplacement(HKEY hKey, LPCSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
{
	const int numParams = 4;
	char *functionName = "RegSaveKeyExA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpFile;
	void *params[numParams] = { &hKey, &str, &lpSecurityAttributes, &Flags };
	ParameterType paramTypes[numParams] = { PointerType, StringType, PointerType, UnsignedLongType };

	if (realRegSaveKeyExA == NULL)
		realRegSaveKeyExA = (RegSaveKeyExAFunction)library->GetOriginalFunction(functionName);

	if (realRegSaveKeyExA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegSaveKeyExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSaveKeyExWReplacement
// Description: See MSDN RegSaveKeyEx function
// Parameters:	See MSDN RegSaveKeyEx parameters
// Return Value: See MSDN RegSaveKeyEx return value
//*************************************************************************
LONG APIENTRY RegSaveKeyExWReplacement(HKEY hKey, LPCSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
{
	const int numParams = 4;
	char *functionName = "RegSaveKeyExW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpFile;
	void *params[numParams] = { &hKey, &str, &lpSecurityAttributes, &Flags };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerType, UnsignedLongType };

	if (realRegSaveKeyExW == NULL)
		realRegSaveKeyExW = (RegSaveKeyExWFunction)library->GetOriginalFunction(functionName);

	if (realRegSaveKeyExW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegSaveKeyExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSaveKeyWReplacement
// Description: See MSDN RegSaveKey function
// Parameters:	See MSDN RegSaveKey parameters
// Return Value: See MSDN RegSaveKey return value
//*************************************************************************
LONG APIENTRY RegSaveKeyWReplacement(HKEY hKey, LPCWSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "RegSaveKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpFile;
	void *params[numParams] = { &hKey, &str, &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerType };

	if (realRegSaveKeyW == NULL)
		realRegSaveKeyW = (RegSaveKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegSaveKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, RegSaveKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSetKeySecurityReplacement
// Description: See MSDN RegSetKeySecurity function
// Parameters:	See MSDN RegSetKeySecurity parameters
// Return Value: See MSDN RegSetKeySecurity return value
//*************************************************************************
LONG APIENTRY RegSetKeySecurityReplacement(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	const int numParams = 3;
	char *functionName = "RegSetKeySecurity";
	char *categoryName = "REGISTRY";
	void *params[numParams] = { &hKey, &SecurityInformation, &pSecurityDescriptor };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };

	if (realRegSetKeySecurity == NULL)
		realRegSetKeySecurity = (RegSetKeySecurityFunction)library->GetOriginalFunction(functionName);

	if (realRegSetKeySecurity != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hKey));
		library->RunStandardTestsAndGetResults(logSender, RegSetKeySecurityCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSetValueAReplacement
// Description: See MSDN RegSetValue function
// Parameters:	See MSDN RegSetValue parameters
// Return Value: See MSDN RegSetValue return value
//*************************************************************************
LONG APIENTRY RegSetValueAReplacement(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
	const int numParams = 5;
	char *functionName = "RegSetValueA";
	char *categoryName = "REGISTRY";
	SiString str[] = { (char *)lpSubKey, (char *)lpData };
	void *params[numParams] = { &hKey, &str[0], &dwType, &str[1], &cbData };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType, StringType, UnsignedLongType };

	if (realRegSetValueA == NULL)
		realRegSetValueA = (RegSetValueAFunction)library->GetOriginalFunction(functionName);

	if (realRegSetValueA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegSetValueACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSetValueExAReplacement
// Description: See MSDN RegSetValueEx function
// Parameters:	See MSDN RegSetValueEx parameters
// Return Value: See MSDN RegSetValueEx return value
//*************************************************************************
LONG APIENTRY RegSetValueExAReplacement(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	const int numParams = 6;
	char *functionName = "RegSetValueExA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpValueName;
	void *params[numParams] = { &hKey, &str, &Reserved, &dwType, &lpData, &cbData };
	ParameterType paramTypes[numParams] = { PointerType, StringType, UnsignedLongType, UnsignedLongType, PointerType, UnsignedLongType };

	if (realRegSetValueExA == NULL)
		realRegSetValueExA = (RegSetValueExAFunction)library->GetOriginalFunction(functionName);

	if (realRegSetValueExA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegSetValueExACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSetValueExWReplacement
// Description: See MSDN RegSetValueEx function
// Parameters:	See MSDN RegSetValueEx parameters
// Return Value: See MSDN RegSetValueEx return value
//*************************************************************************
LONG APIENTRY RegSetValueExWReplacement(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	const int numParams = 6;
	char *functionName = "RegSetValueExW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpValueName;
	void *params[numParams] = { &hKey, &str, &Reserved, &dwType, &lpData, &cbData };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType, UnsignedLongType, PointerType, UnsignedLongType };

	if (realRegSetValueExW == NULL)
		realRegSetValueExW = (RegSetValueExWFunction)library->GetOriginalFunction(functionName);

	if (realRegSetValueExW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpValueName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegSetValueExWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegSetValueWReplacement
// Description: See MSDN RegSetValue function
// Parameters:	See MSDN RegSetValue parameters
// Return Value: See MSDN RegSetValue return value
//*************************************************************************
LONG APIENTRY RegSetValueWReplacement(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
	const int numParams = 5;
	char *functionName = "RegSetValueW";
	char *categoryName = "REGISTRY";
	SiString str[] = { (wchar_t *)lpSubKey, (wchar_t *)lpData };
	void *params[numParams] = { &hKey, &str[0], &dwType, &str[1], &cbData };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, UnsignedLongType, WideStringType, UnsignedLongType };

	if (realRegSetValueW == NULL)
		realRegSetValueW = (RegSetValueWFunction)library->GetOriginalFunction(functionName);

	if (realRegSetValueW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegSetValueWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegUnLoadKeyAReplacement
// Description: See MSDN RegUnLoadKey function
// Parameters:	See MSDN RegUnLoadKey parameters
// Return Value: See MSDN RegUnLoadKey return value
//*************************************************************************
LONG APIENTRY RegUnLoadKeyAReplacement(HKEY hKey, LPCSTR lpSubKey)
{
	const int numParams = 2;
	char *functionName = "RegUnLoadKeyA";
	char *categoryName = "REGISTRY";
	SiString str = (char *)lpSubKey;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, StringType };

	if (realRegUnLoadKeyA == NULL)
		realRegUnLoadKeyA = (RegUnLoadKeyAFunction)library->GetOriginalFunction(functionName);

	if (realRegUnLoadKeyA != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegUnLoadKeyACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RegUnLoadKeyWReplacement
// Description: See MSDN RegUnLoadKey function
// Parameters:	See MSDN RegUnLoadKey parameters
// Return Value: See MSDN RegUnLoadKey return value
//*************************************************************************
LONG APIENTRY RegUnLoadKeyWReplacement(HKEY hKey, LPCWSTR lpSubKey)
{
	const int numParams = 2;
	char *functionName = "RegUnLoadKeyW";
	char *categoryName = "REGISTRY";
	SiString str = (wchar_t *)lpSubKey;
	void *params[numParams] = { &hKey, &str };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };

	if (realRegUnLoadKeyW == NULL)
		realRegUnLoadKeyW = (RegUnLoadKeyWFunction)library->GetOriginalFunction(functionName);

	if (realRegUnLoadKeyW != NULL)
	{
		DWORD errorCode, returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(hKey, lpSubKey);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		library->RunStandardTestsAndGetResults(logSender, RegUnLoadKeyWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "LONG", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		BackupEventLogAReplacement
// Description: See MSDN BackupEventLogA function
// Parameters:	See MSDN BackupEventLogA parameters
// Return Value: See MSDN BackupEventLogA return value
//*************************************************************************
BOOL WINAPI BackupEventLogAReplacement(HANDLE hEventLog, LPCSTR lpBackupFileName)
{
	const int numParams = 2;
	char *functionName = "BackupEventLogA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpBackupFileName };
	void *params[numParams] = { &hEventLog, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, StringType };
	
	if (realBackupEventLogA == NULL)
		realBackupEventLogA = (BackupEventLogAFunction)library->GetOriginalFunction(functionName);
		
	if (realBackupEventLogA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, BackupEventLogACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		BackupEventLogWReplacement
// Description: See MSDN BackupEventLogW function
// Parameters:	See MSDN BackupEventLogW parameters
// Return Value: See MSDN BackupEventLogW return value
//*************************************************************************
BOOL WINAPI BackupEventLogWReplacement(HANDLE hEventLog, LPCWSTR lpBackupFileName)
{
	const int numParams = 2;
	char *functionName = "BackupEventLogW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpBackupFileName };
	void *params[numParams] = { &hEventLog, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };
	
	if (realBackupEventLogW == NULL)
		realBackupEventLogW = (BackupEventLogWFunction)library->GetOriginalFunction(functionName);
		
	if (realBackupEventLogW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, BackupEventLogWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ClearEventLogAReplacement
// Description: See MSDN ClearEventLogA function
// Parameters:	See MSDN ClearEventLogA parameters
// Return Value: See MSDN ClearEventLogA return value
//*************************************************************************
BOOL WINAPI ClearEventLogAReplacement(HANDLE hEventLog, LPCSTR lpBackupFileName)
{
	const int numParams = 2;
	char *functionName = "ClearEventLogA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpBackupFileName };
	void *params[numParams] = { &hEventLog, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, StringType };
	
	if (realClearEventLogA == NULL)
		realClearEventLogA = (ClearEventLogAFunction)library->GetOriginalFunction(functionName);
		
	if (realClearEventLogA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ClearEventLogACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ClearEventLogWReplacement
// Description: See MSDN ClearEventLogW function
// Parameters:	See MSDN ClearEventLogW parameters
// Return Value: See MSDN ClearEventLogW return value
//*************************************************************************
BOOL WINAPI ClearEventLogWReplacement(HANDLE hEventLog, LPCWSTR lpBackupFileName)
{
	const int numParams = 2;
	char *functionName = "ClearEventLogW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpBackupFileName };
	void *params[numParams] = { &hEventLog, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };
	
	if (realClearEventLogW == NULL)
		realClearEventLogW = (ClearEventLogWFunction)library->GetOriginalFunction(functionName);
		
	if (realClearEventLogW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ClearEventLogWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CloseEventLogReplacement
// Description: See MSDN CloseEventLog function
// Parameters:	See MSDN CloseEventLog parameters
// Return Value: See MSDN CloseEventLog return value
//*************************************************************************
BOOL WINAPI CloseEventLogReplacement(HANDLE hEventLog)
{
	const int numParams = 1;
	char *functionName = "CloseEventLog";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hEventLog };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realCloseEventLog == NULL)
		realCloseEventLog = (CloseEventLogFunction)library->GetOriginalFunction(functionName);
		
	if (realCloseEventLog != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CloseEventLogCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method: CreateProcessAsUserAReplacement
// Description: See MSDN CreateProcessAsUser function
// Parameters:	See MSDN CreateProcessAsUser parameters
// Return Value: See MSDN CreateProcessAsUser return value
//*************************************************************************
BOOL WINAPI CreateProcessAsUserAReplacement(HANDLE hToken, LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();

	const int numParams = 11;
	char *functionName = "CreateProcessAsUserA";
	char *categoryName = "PROCESS";
	SiString str[] = { (char *)lpApplicationName, (char *)lpCommandLine, (char *)lpCurrentDirectory };
	void *params[numParams] = { &hToken, &str[0], &str[1], &lpProcessAttributes, &lpThreadAttributes, &bInheritHandles, &dwCreationFlags, &lpEnvironment, &str[2], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = { PointerType, StringType, StringType, PointerType, PointerType, BooleanType, UnsignedLongType, PointerType, StringType, PointerType, PointerType };

	if (realCreateProcessAsUserA == NULL)
		realCreateProcessAsUserA = (CreateProcessAsUserAFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessAsUserA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessAsUserACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
			NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method: CreateProcessAsUserWReplacement
// Description: See MSDN CreateProcessAsUser function
// Parameters:	See MSDN CreateProcessAsUser parameters
// Return Value: See MSDN CreateProcessAsUser return value
//*************************************************************************
BOOL WINAPI CreateProcessAsUserWReplacement(HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();

	const int numParams = 11;
	char *functionName = "CreateProcessAsUserW";
	char *categoryName = "PROCESS";
	SiString str[] = { (wchar_t *)lpApplicationName, (wchar_t *)lpCommandLine, (wchar_t *)lpCurrentDirectory };
	void *params[numParams] = { &hToken, &str[0], &str[1], &lpProcessAttributes, &lpThreadAttributes, &bInheritHandles, &dwCreationFlags, &lpEnvironment, &str[2], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, WideStringType, PointerType, PointerType, BooleanType, UnsignedLongType, PointerType, WideStringType, PointerType, PointerType };

	if (realCreateProcessAsUserW == NULL)
		realCreateProcessAsUserW = (CreateProcessAsUserWFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessAsUserW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessAsUserWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
			NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method: CreateProcessWithLogonWReplacement
// Description: See MSDN CreateProcessWithLogonW function
// Parameters:	See MSDN CreateProcessWithLogonW parameters
// Return Value: See MSDN CreateProcessWithLogonW return value
//*************************************************************************
BOOL WINAPI CreateProcessWithLogonWReplacement(LPCWSTR lpUsername, LPCWSTR lpDomain, LPCWSTR lpPassword, DWORD dwLogonFlags, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	createProcessMutex->Acquire();

	const int numParams = 11;
	char *functionName = "CreateProcessWithLogonW";
	char *categoryName = "PROCESS";
	SiString str[] = { (wchar_t *)lpUsername, (wchar_t *)lpDomain, (wchar_t *)lpPassword, (wchar_t *)lpApplicationName, (wchar_t *)lpCommandLine, (wchar_t *)lpCurrentDirectory };
	void *params[numParams] = { &str[0], &str[1], &str[2], &dwLogonFlags, &str[3], &str[4], &dwCreationFlags, &lpEnvironment, &str[5], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, WideStringType, UnsignedLongType, WideStringType, WideStringType, UnsignedLongType, PointerType, WideStringType, PointerType, PointerType };

	if (realCreateProcessWithLogonW == NULL)
		realCreateProcessWithLogonW = (CreateProcessWithLogonWFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessWithLogonW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessWithLogonWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
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

	const int numParams = 9;
	char *functionName = "CreateProcessWithTokenW";
	char *categoryName = "PROCESS";
	SiString str[] = { (wchar_t *)lpApplicationName, (wchar_t *)lpCommandLine, (wchar_t *)lpCurrentDirectory };
	void *params[numParams] = { &hToken, &dwLogonFlags, &str[0], &str[1], &dwCreationFlags, &lpEnvironment, &str[2], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = {PointerType, UnsignedLongType, WideStringType, WideStringType, UnsignedLongType, PointerType, WideStringType, PointerType, PointerType };


	if (realCreateProcessWithTokenW == NULL)
		realCreateProcessWithTokenW = (CreateProcessWithTokenWFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessWithTokenW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessWithTokenWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
			NotifyHolodeckOfCreatedProcess(dwCreationFlags, lpProcessInformation->dwProcessId);

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		DecryptFileAReplacement
// Description: See MSDN DecryptFileA function
// Parameters:	See MSDN DecryptFileA parameters
// Return Value: See MSDN DecryptFileA return value
//*************************************************************************
BOOL WINAPI DecryptFileAReplacement(LPCSTR lpFileName, DWORD dwReserved)
{
	const int numParams = 2;
	char *functionName = "DecryptFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &dwReserved };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType };
	
	if (realDecryptFileA == NULL)
		realDecryptFileA = (DecryptFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realDecryptFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DecryptFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		DecryptFileWReplacement
// Description: See MSDN DecryptFileW function
// Parameters:	See MSDN DecryptFileW parameters
// Return Value: See MSDN DecryptFileW return value
//*************************************************************************
BOOL WINAPI DecryptFileWReplacement(LPCWSTR lpFileName, DWORD dwReserved)
{
	const int numParams = 2;
	char *functionName = "DecryptFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &dwReserved };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType };
	
	if (realDecryptFileW == NULL)
		realDecryptFileW = (DecryptFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realDecryptFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DecryptFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		EncryptFileAReplacement
// Description: See MSDN EncryptFileA function
// Parameters:	See MSDN EncryptFileA parameters
// Return Value: See MSDN EncryptFileA return value
//*************************************************************************
BOOL WINAPI EncryptFileAReplacement(LPCSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "EncryptFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realEncryptFileA == NULL)
		realEncryptFileA = (EncryptFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realEncryptFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, EncryptFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		EncryptFileWReplacement
// Description: See MSDN EncryptFileW function
// Parameters:	See MSDN EncryptFileW parameters
// Return Value: See MSDN EncryptFileW return value
//*************************************************************************
BOOL WINAPI EncryptFileWReplacement(LPCWSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "EncryptFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realEncryptFileW == NULL)
		realEncryptFileW = (EncryptFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realEncryptFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, EncryptFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		FileEncryptionStatusAReplacement
// Description: See MSDN FileEncryptionStatusA function
// Parameters:	See MSDN FileEncryptionStatusA parameters
// Return Value: See MSDN FileEncryptionStatusA return value
//*************************************************************************
BOOL WINAPI FileEncryptionStatusAReplacement(LPCSTR lpFileName, LPDWORD lpStatus)
{
	const int numParams = 2;
	char *functionName = "FileEncryptionStatusA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &lpStatus };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realFileEncryptionStatusA == NULL)
		realFileEncryptionStatusA = (FileEncryptionStatusAFunction)library->GetOriginalFunction(functionName);
		
	if (realFileEncryptionStatusA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FileEncryptionStatusACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		FileEncryptionStatusWReplacement
// Description: See MSDN FileEncryptionStatusW function
// Parameters:	See MSDN FileEncryptionStatusW parameters
// Return Value: See MSDN FileEncryptionStatusW return value
//*************************************************************************
BOOL WINAPI FileEncryptionStatusWReplacement(LPCWSTR lpFileName, LPDWORD lpStatus)
{
	const int numParams = 2;
	char *functionName = "FileEncryptionStatusW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &lpStatus };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realFileEncryptionStatusW == NULL)
		realFileEncryptionStatusW = (FileEncryptionStatusWFunction)library->GetOriginalFunction(functionName);
		
	if (realFileEncryptionStatusW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FileEncryptionStatusWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetFileSecurityAReplacement
// Description: See MSDN GetFileSecurityA function
// Parameters:	See MSDN GetFileSecurityA parameters
// Return Value: See MSDN GetFileSecurityA return value
//*************************************************************************
BOOL WINAPI GetFileSecurityAReplacement(LPCSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded)
{
	const int numParams = 5;
	char *functionName = "GetFileSecurityA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &RequestedInformation, &pSecurityDescriptor, &nLength, &lpnLengthNeeded };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType, PointerType, UnsignedLongType, PointerType };
	
	if (realGetFileSecurityA == NULL)
		realGetFileSecurityA = (GetFileSecurityAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileSecurityA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileSecurityACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetFileSecurityWReplacement
// Description: See MSDN GetFileSecurityW function
// Parameters:	See MSDN GetFileSecurityW parameters
// Return Value: See MSDN GetFileSecurityW return value
//*************************************************************************
BOOL WINAPI GetFileSecurityWReplacement(LPCWSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded)
{
	const int numParams = 5;
	char *functionName = "GetFileSecurityW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &RequestedInformation, &pSecurityDescriptor, &nLength, &lpnLengthNeeded };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType, PointerType, UnsignedLongType, PointerType };
	
	if (realGetFileSecurityW == NULL)
		realGetFileSecurityW = (GetFileSecurityWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileSecurityW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileSecurityWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		OpenBackupEventLogAReplacement
// Description: See MSDN OpenBackupEventLogA function
// Parameters:	See MSDN OpenBackupEventLogA parameters
// Return Value: See MSDN OpenBackupEventLogA return value
//*************************************************************************
HANDLE WINAPI OpenBackupEventLogAReplacement(LPCSTR lpUNCServerName, LPCSTR lpFileName)
{
	const int numParams = 2;
	char *functionName = "OpenBackupEventLogA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpUNCServerName, (char *)lpFileName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { StringType, StringType };
	
	if (realOpenBackupEventLogA == NULL)
		realOpenBackupEventLogA = (OpenBackupEventLogAFunction)library->GetOriginalFunction(functionName);
		
	if (realOpenBackupEventLogA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(lpUNCServerName, lpFileName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		
		if (library->RunStandardTestsAndGetResults(logSender, OpenBackupEventLogACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		OpenBackupEventLogWReplacement
// Description: See MSDN OpenBackupEventLogW function
// Parameters:	See MSDN OpenBackupEventLogW parameters
// Return Value: See MSDN OpenBackupEventLogW return value
//*************************************************************************
HANDLE WINAPI OpenBackupEventLogWReplacement(LPCWSTR lpUNCServerName, LPCWSTR lpFileName)
{
	const int numParams = 2;
	char *functionName = "OpenBackupEventLogW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpUNCServerName, (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType };
	
	if (realOpenBackupEventLogW == NULL)
		realOpenBackupEventLogW = (OpenBackupEventLogWFunction)library->GetOriginalFunction(functionName);
		
	if (realOpenBackupEventLogW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(lpUNCServerName, lpFileName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		
		if (library->RunStandardTestsAndGetResults(logSender, OpenBackupEventLogWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		OpenEventLogAReplacement
// Description: See MSDN OpenEventLogA function
// Parameters:	See MSDN OpenEventLogA parameters
// Return Value: See MSDN OpenEventLogA return value
//*************************************************************************
HANDLE WINAPI OpenEventLogAReplacement(LPCSTR lpUNCServerName, LPCSTR lpSourceName)
{
	const int numParams = 2;
	char *functionName = "OpenEventLogA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpUNCServerName, (char *)lpSourceName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { StringType, StringType };
	
	if (realOpenEventLogA == NULL)
		realOpenEventLogA = (OpenEventLogAFunction)library->GetOriginalFunction(functionName);
		
	if (realOpenEventLogA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(lpUNCServerName, lpSourceName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		
		if (library->RunStandardTestsAndGetResults(logSender, OpenEventLogACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		OpenEventLogWReplacement
// Description: See MSDN OpenEventLogW function
// Parameters:	See MSDN OpenEventLogW parameters
// Return Value: See MSDN OpenEventLogW return value
//*************************************************************************
HANDLE WINAPI OpenEventLogWReplacement(LPCWSTR lpUNCServerName, LPCWSTR lpSourceName)
{
	const int numParams = 2;
	char *functionName = "OpenEventLogW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpUNCServerName, (wchar_t *)lpSourceName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType };
	
	if (realOpenEventLogW == NULL)
		realOpenEventLogW = (OpenEventLogWFunction)library->GetOriginalFunction(functionName);
		
	if (realOpenEventLogW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		//stitch together resource path
		SiString resourcePath = library->ConcatResourcePath(lpUNCServerName, lpSourceName);
		//add to pathlist for use by doestestmatch
		library->AddToPathList(1, resourcePath);
		
		if (library->RunStandardTestsAndGetResults(logSender, OpenEventLogWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		QueryUsersOnEncryptedFileReplacement
// Description: See MSDN QueryUsersOnEncryptedFile function
// Parameters:	See MSDN QueryUsersOnEncryptedFile parameters
// Return Value: See MSDN QueryUsersOnEncryptedFile return value
//*************************************************************************
DWORD WINAPI QueryUsersOnEncryptedFileReplacement(LPCWSTR lpFileName, PENCRYPTION_CERTIFICATE_HASH_LIST * pUsers)
{
	const int numParams = 2;
	char *functionName = "QueryUsersOnEncryptedFile";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &pUsers };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realQueryUsersOnEncryptedFile == NULL)
		realQueryUsersOnEncryptedFile = (QueryUsersOnEncryptedFileFunction)library->GetOriginalFunction(functionName);
		
	if (realQueryUsersOnEncryptedFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, QueryUsersOnEncryptedFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		RemoveUsersFromEncryptedFileReplacement
// Description: See MSDN RemoveUsersFromEncryptedFile function
// Parameters:	See MSDN RemoveUsersFromEncryptedFile parameters
// Return Value: See MSDN RemoveUsersFromEncryptedFile return value
//*************************************************************************
DWORD WINAPI RemoveUsersFromEncryptedFileReplacement(LPCWSTR lpFileName, PENCRYPTION_CERTIFICATE_HASH_LIST pHashes)
{
	const int numParams = 2;
	char *functionName = "RemoveUsersFromEncryptedFile";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &pHashes };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realRemoveUsersFromEncryptedFile == NULL)
		realRemoveUsersFromEncryptedFile = (RemoveUsersFromEncryptedFileFunction)library->GetOriginalFunction(functionName);
		
	if (realRemoveUsersFromEncryptedFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, RemoveUsersFromEncryptedFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SaferRecordEventLogEntryReplacement
// Description: See MSDN SaferRecordEventLogEntry function
// Parameters:	See MSDN SaferRecordEventLogEntry parameters
// Return Value: See MSDN SaferRecordEventLogEntry return value
//*************************************************************************
BOOL WINAPI SaferRecordEventLogEntryReplacement(SAFER_LEVEL_HANDLE hLevel, LPCWSTR szTargetPath, LPVOID lpReserved)
{
	const int numParams = 3;
	char *functionName = "SaferRecordEventLogEntry";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)szTargetPath };
	void *params[numParams] = { &hLevel, &str[0], &lpReserved };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, PointerType };
	
	if (realSaferRecordEventLogEntry == NULL)
		realSaferRecordEventLogEntry = (SaferRecordEventLogEntryFunction)library->GetOriginalFunction(functionName);
		
	if (realSaferRecordEventLogEntry != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SaferRecordEventLogEntryCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SaferiIsExecutableFileTypeReplacement
// Description: See MSDN SaferiIsExecutableFileType function
// Parameters:	See MSDN SaferiIsExecutableFileType parameters
// Return Value: See MSDN SaferiIsExecutableFileType return value
//*************************************************************************
BOOL WINAPI SaferiIsExecutableFileTypeReplacement(LPCWSTR szFullPathname, BOOLEAN bFromShellExecute)
{
	const int numParams = 2;
	char *functionName = "SaferiIsExecutableFileType";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)szFullPathname };
	void *params[numParams] = { &str[0], &bFromShellExecute };
	ParameterType paramTypes[numParams] = { WideStringType, BooleanType };
	
	if (realSaferiIsExecutableFileType == NULL)
		realSaferiIsExecutableFileType = (SaferiIsExecutableFileTypeFunction)library->GetOriginalFunction(functionName);
		
	if (realSaferiIsExecutableFileType != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SaferiIsExecutableFileTypeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SetFileSecurityAReplacement
// Description: See MSDN SetFileSecurityA function
// Parameters:	See MSDN SetFileSecurityA parameters
// Return Value: See MSDN SetFileSecurityA return value
//*************************************************************************
BOOL WINAPI SetFileSecurityAReplacement(LPCSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	const int numParams = 3;
	char *functionName = "SetFileSecurityA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &SecurityInformation, &pSecurityDescriptor };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType, PointerType };
	
	if (realSetFileSecurityA == NULL)
		realSetFileSecurityA = (SetFileSecurityAFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileSecurityA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileSecurityACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SetFileSecurityWReplacement
// Description: See MSDN SetFileSecurityW function
// Parameters:	See MSDN SetFileSecurityW parameters
// Return Value: See MSDN SetFileSecurityW return value
//*************************************************************************
BOOL WINAPI SetFileSecurityWReplacement(LPCWSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	const int numParams = 3;
	char *functionName = "SetFileSecurityW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &SecurityInformation, &pSecurityDescriptor };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType, PointerType };
	
	if (realSetFileSecurityW == NULL)
		realSetFileSecurityW = (SetFileSecurityWFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileSecurityW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileSecurityWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SetSecurityDescriptorDaclReplacement
// Description: See MSDN SetSecurityDescriptorDacl function
// Parameters:	See MSDN SetSecurityDescriptorDacl parameters
// Return Value: See MSDN SetSecurityDescriptorDacl return value
//*************************************************************************
BOOL WINAPI SetSecurityDescriptorDaclReplacement(PSECURITY_DESCRIPTOR pSecurityDescriptor, BOOL bDaclPresent, PACL pDacl, BOOL bDaclDefaulted)
{
	const int numParams = 4;
	char *functionName = "SetSecurityDescriptorDacl";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &pSecurityDescriptor, &bDaclPresent, &pDacl, &bDaclDefaulted };
	ParameterType paramTypes[numParams] = { PointerType, BooleanType, PointerType, BooleanType };
	
	if (realSetSecurityDescriptorDacl == NULL)
		realSetSecurityDescriptorDacl = (SetSecurityDescriptorDaclFunction)library->GetOriginalFunction(functionName);
		
	if (realSetSecurityDescriptorDacl != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetSecurityDescriptorDaclCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// END OF REPLACEMENT FUNCTIONS
//*************************************************************************
