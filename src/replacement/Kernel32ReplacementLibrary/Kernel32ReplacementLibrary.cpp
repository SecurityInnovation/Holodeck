//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		Kernel32ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for kernel32.dll
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Generated 05/29/2004 04:59:55 PM
//*************************************************************************
#include "NetCorruptLibrary.h"
#include "kernel32replacementlibrary.h"
#include "CLRIntercept.h"

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
		//Initialize intercepts
		CLRInitializeIntercepts();
		
		Registry reg;

		SiArray<SiString> externalLibs;
		//Check for any external dotnet libraries have been specified and then cache them
		if (reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		{
			DWORD pid = GetCurrentProcessId();
			char pidString[12];
			_ultoa(pid, pidString, 10);
			
			SiString extLibPath = pidString;
			extLibPath += "\\";
			extLibPath += DOTNET_LIBS_INIT_LIST_KEY;

			externalLibs = reg.GetValueNames (reg.GetCurrentKey(), extLibPath);

			for (unsigned int j=0; j < externalLibs.GetLength(); j++)
			{
				SiString eLibs = externalLibs[j];
				CLRAddInterceptHandlerModuleForCacheCheck((char *) eLibs);
			}
		}
		
		
		//Cache user generated dotnet replacement libraries and Holodeck internal
		//dotnet replacement libraries only if no external libs were initialized above
		if (externalLibs.GetLength() == 0)
		{
			SiString sDotNetReplacementKey;
			SiArray<SiString> interceptedDLLs;
			if (reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
			{
				interceptedDLLs = reg.GetValueNames (reg.GetCurrentKey(), DOTNET_REPLACEMENT_DLLS_KEY);

				for (unsigned int j=0; j<interceptedDLLs.GetLength(); j++)
				{
					SiString iDLL = interceptedDLLs[j];
					CLRAddInterceptHandlerModuleForCacheCheck((char *) iDLL);
				}
			}

			//Cache holodeck dotnet replacement libraries
			CLRAddInterceptHandlerModuleForCacheCheck("MSCorLibReplacementLibrary.dll");
			CLRAddInterceptHandlerModuleForCacheCheck("SystemReplacementLibrary.dll");
			CLRAddInterceptHandlerModuleForCacheCheck("SystemServiceProcessReplacementLibrary.dll");
			CLRAddInterceptHandlerModuleForCacheCheck("SystemXMLReplacementLibrary.dll");
		}

		library = new ReplacementLibrary("kernel32.dll");
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

		InitNetworkCorruption();

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
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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
// Method: AllocateUserPhysicalPagesCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool AllocateUserPhysicalPagesCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realAllocateUserPhysicalPages(*((HANDLE *)params[0]), *((PULONG_PTR *)params[1]), *((PULONG_PTR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CopyFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CopyFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCopyFileA(*((SiString *)params[0]), *((SiString *)params[1]), *((BOOL *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CopyFileExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CopyFileExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realCopyFileExA(*((SiString *)params[0]), *((SiString *)params[1]), *((LPPROGRESS_ROUTINE *)params[2]), *((LPVOID *)params[3]), *((LPBOOL *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CopyFileExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CopyFileExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realCopyFileExW(*((SiString *)params[0]), *((SiString *)params[1]), *((LPPROGRESS_ROUTINE *)params[2]), *((LPVOID *)params[3]), *((LPBOOL *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CopyFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CopyFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCopyFileW(*((SiString *)params[0]), *((SiString *)params[1]), *((BOOL *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateDirectoryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateDirectoryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realCreateDirectoryA(*((SiString *)params[0]), *((LPSECURITY_ATTRIBUTES *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateDirectoryExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateDirectoryExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCreateDirectoryExA(*((SiString *)params[0]), *((SiString *)params[1]), *((LPSECURITY_ATTRIBUTES *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateDirectoryExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateDirectoryExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCreateDirectoryExW(*((SiString *)params[0]), *((SiString *)params[1]), *((LPSECURITY_ATTRIBUTES *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateDirectoryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateDirectoryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realCreateDirectoryW(*((SiString *)params[0]), *((LPSECURITY_ATTRIBUTES *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;

	SiString replacementBinary = "";
	HANDLE tempRetValue;

	// Only try to do .NET binary modification if the file name ends in ".dll", ".netmodule",
	// or ".ni" (native image of .NET assembly)
	if (((SiString*)params[0])->GetLength() > 3)
	{
		SiString ext = "";

		int dotIndex = ((SiString*)params[0])->FindLast('.');
		if (dotIndex != -1)
			((SiString*)params[0])->SubString(ext, dotIndex);
		if ((_stricmp((char*)ext, ".dll") == 0) || (_stricmp((char*)ext, ".netmodule") == 0) ||
			(_stricmp((char*)ext, ".ni") == 0))
		{
			// .NET interception: We need to load a modified binary if the module being loaded
			// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
			// to intercept the required .NET methods, if any, and call the real LoadLibrary
			// on the modified binary if there were intercepts.  Doing it here in the caller
			// functions prevents the modified binary path from showing up in the log, so that
			// the user sees the original module path.
			if (!CLRAllowModuleLoad(*((SiString *)params[0])))
			{
				// CLRAllowModuleLoad returned false, meaning that the module was the native
				// form of a .NET binary that we are intercepting.  By denying the loading of
				// this module, we can force the .NET runtime to recompile the intercepted
				// module, allowing our interception to take place
				*retValue = -1;
				*errCode = ERROR_FILE_NOT_FOUND;
				return true;
			}

			replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
		}
	}

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, CreateFile should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
	{
		tempRetValue = FileCorrupt_CreateFileA(*((SiString *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), 
			*((LPSECURITY_ATTRIBUTES *)params[3]), *((DWORD *)params[4]), *((DWORD *)params[5]), *((HANDLE *)params[6]));
	}
	else
	{
		tempRetValue = CreateFileA(replacementBinary, *((DWORD *)params[1]), *((DWORD *)params[2]), 
			*((LPSECURITY_ATTRIBUTES *)params[3]), *((DWORD *)params[4]), *((DWORD *)params[5]), *((HANDLE *)params[6]));
	}

	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;

	SiString replacementBinary = "";
	HANDLE temp;

	// Only try to do .NET binary modification if the file name ends in ".dll", ".netmodule",
	// or ".ni" (native image of .NET assembly)
	if (((SiString*)params[0])->GetLength() > 3)
	{
		SiString ext = "";

		int dotIndex = ((SiString*)params[0])->FindLast('.');
		if (dotIndex != -1)
			((SiString*)params[0])->SubString(ext, dotIndex);
		if ((_stricmp((char*)ext, ".dll") == 0) || (_stricmp((char*)ext, ".netmodule") == 0) ||
			(_stricmp((char*)ext, ".ni") == 0))
		{
			// .NET interception: We need to load a modified binary if the module being loaded
			// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
			// to intercept the required .NET methods, if any, and call the real LoadLibrary
			// on the modified binary if there were intercepts.  Doing it here in the caller
			// functions prevents the modified binary path from showing up in the log, so that
			// the user sees the original module path.
			if (!CLRAllowModuleLoad(*((SiString *)params[0])))
			{
				// CLRAllowModuleLoad returned false, meaning that the module was the native
				// form of a .NET binary that we are intercepting.  By denying the loading
				// this module, we can force the .NET runtime to recompile the intercepted
				// module, allowing our interception to take place
				*retValue = -1;
				*errCode = ERROR_FILE_NOT_FOUND;
				return true;
			}

			replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
		}
	}

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, CreateFile should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
	{
		temp = FileCorrupt_CreateFileW(*((SiString *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]),
			*((DWORD *)params[4]), *((DWORD *)params[5]), *((HANDLE *)params[6]));
	}
	else
	{
		temp = CreateFileW(replacementBinary, *((DWORD *)params[1]), *((DWORD *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]),
			*((DWORD *)params[4]), *((DWORD *)params[5]), *((HANDLE *)params[6]));
	}

	memcpy(retValue, &temp, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 10) return false;

	// always create suspended.  if the original flags were not suspended
	*retValue = realCreateProcessA(*((SiString *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]),
		*((BOOL *)params[4]), *((DWORD *)params[5]) | CREATE_SUSPENDED, *((LPVOID *)params[6]),
		*((SiString *)params[7]), *((LPSTARTUPINFOA *)params[8]), *((LPPROCESS_INFORMATION *)params[9]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateProcessWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateProcessWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 10) return false;

	// always create suspended.  if the original flags were not suspended
	*retValue = realCreateProcessW(*((SiString *)params[0]), *((SiString *)params[1]), 
		*((LPSECURITY_ATTRIBUTES *)params[2]), *((LPSECURITY_ATTRIBUTES *)params[3]),
		*((BOOL *)params[4]), *((DWORD *)params[5]) | CREATE_SUSPENDED, *((LPVOID *)params[6]),
		*((SiString *)params[7]), *((LPSTARTUPINFOW *)params[8]), *((LPPROCESS_INFORMATION *)params[9]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DeleteFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DeleteFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDeleteFileA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DeleteFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DeleteFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDeleteFileW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FlushFileBuffersCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FlushFileBuffersCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realFlushFileBuffers(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FreeLibraryCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FreeLibraryCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realFreeLibrary(*((HMODULE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FreeLibraryAndExitThreadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FreeLibraryAndExitThreadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	realFreeLibraryAndExitThread(*((HMODULE *)params[0]), *((DWORD *)params[1]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FreeUserPhysicalPagesCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FreeUserPhysicalPagesCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realFreeUserPhysicalPages(*((HANDLE *)params[0]), *((PULONG_PTR *)params[1]), *((PULONG_PTR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetBinaryTypeACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetBinaryTypeACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetBinaryTypeA(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetBinaryTypeWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetBinaryTypeWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetBinaryTypeW(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetCurrentDirectoryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetCurrentDirectoryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetCurrentDirectoryA(*((DWORD *)params[0]), *((LPSTR *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetCurrentDirectoryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetCurrentDirectoryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetCurrentDirectoryW(*((DWORD *)params[0]), *((LPWSTR *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDiskFreeSpaceACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDiskFreeSpaceACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realGetDiskFreeSpaceA(*((SiString *)params[0]), *((LPDWORD *)params[1]), *((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDiskFreeSpaceExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDiskFreeSpaceExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realGetDiskFreeSpaceExA(*((SiString *)params[0]), *((PULARGE_INTEGER *)params[1]), *((PULARGE_INTEGER *)params[2]), *((PULARGE_INTEGER *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDiskFreeSpaceExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDiskFreeSpaceExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realGetDiskFreeSpaceExW(*((SiString *)params[0]), *((PULARGE_INTEGER *)params[1]), *((PULARGE_INTEGER *)params[2]), *((PULARGE_INTEGER *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDiskFreeSpaceWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDiskFreeSpaceWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realGetDiskFreeSpaceW(*((SiString *)params[0]), *((LPDWORD *)params[1]), *((LPDWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDriveTypeACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDriveTypeACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	UINT tempRetValue = realGetDriveTypeA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetDriveTypeWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetDriveTypeWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	UINT tempRetValue = realGetDriveTypeW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileAttributesACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileAttributesACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	DWORD tempRetValue = realGetFileAttributesA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileAttributesExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileAttributesExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realGetFileAttributesExA(*((SiString *)params[0]), *((GET_FILEEX_INFO_LEVELS *)params[1]), *((LPVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileAttributesExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileAttributesExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realGetFileAttributesExW(*((SiString *)params[0]), *((GET_FILEEX_INFO_LEVELS *)params[1]), *((LPVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileAttributesWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileAttributesWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	DWORD tempRetValue = realGetFileAttributesW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileInformationByHandleCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileInformationByHandleCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetFileInformationByHandle(*((HANDLE *)params[0]), *((LPBY_HANDLE_FILE_INFORMATION *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileSizeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileSizeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetFileSize(*((HANDLE *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileSizeExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileSizeExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetFileSizeEx(*((HANDLE *)params[0]), *((PLARGE_INTEGER *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileTypeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileTypeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	DWORD tempRetValue = realGetFileType(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetLogicalDrivesCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetLogicalDrivesCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	DWORD tempRetValue = realGetLogicalDrives();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetLogicalDriveStringsACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetLogicalDriveStringsACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetLogicalDriveStringsA(*((DWORD *)params[0]), *((LPSTR *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetLogicalDriveStringsWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetLogicalDriveStringsWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetLogicalDriveStringsW(*((DWORD *)params[0]), *((LPWSTR *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GlobalAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GlobalAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HGLOBAL tempRetValue = realGlobalAlloc(*((UINT *)params[0]), *((SIZE_T *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GlobalFreeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GlobalFreeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HGLOBAL tempRetValue = realGlobalFree(*((HGLOBAL *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GlobalMemoryStatusExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GlobalMemoryStatusExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realGlobalMemoryStatusEx(*((LPMEMORYSTATUSEX *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	LPVOID tempRetValue = realHeapAlloc(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapCompactCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapCompactCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	SIZE_T tempRetValue = realHeapCompact(*((HANDLE *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapCreateCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapCreateCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	HANDLE tempRetValue = realHeapCreate(*((DWORD *)params[0]), *((SIZE_T *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapDestroyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapDestroyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realHeapDestroy(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapFreeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapFreeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realHeapFree(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((LPVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapLockCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapLockCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realHeapLock(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapReAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapReAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	LPVOID tempRetValue = realHeapReAlloc(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((LPVOID *)params[2]), *((SIZE_T *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapSizeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapSizeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	SIZE_T tempRetValue = realHeapSize(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((LPCVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapUnlockCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapUnlockCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realHeapUnlock(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: HeapValidateCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool HeapValidateCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realHeapValidate(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((LPCVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LoadLibraryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LoadLibraryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;

	// .NET interception: We need to load a modified binary if the module being loaded
	// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
	// to intercept the required .NET methods, if any, and call the real LoadLibrary
	// on the modified binary if there were intercepts.  Doing it here in the caller
	// functions prevents the modified binary path from showing up in the log, so that
	// the user sees the original module path.
	if (!CLRAllowModuleLoad(*((SiString *)params[0])))
	{
		// CLRAllowModuleLoad returned false, meaning that the module was the native
		// form of a .NET binary that we are intercepting.  By denying LoadLibrary on
		// this module, we can force the .NET runtime to recompile the intercepted
		// module, allowing our interception to take place
		*retValue = 0;
		*errCode = ERROR_FILE_NOT_FOUND;
		return true;
	}

	SiString replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
	HMODULE temp;

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, LoadLibrary should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
		temp = realLoadLibraryA(*((SiString *)params[0]));
	else
		temp = realLoadLibraryA(replacementBinary);

	memcpy(retValue, &temp, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LoadLibraryExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LoadLibraryExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;

	// .NET interception: We need to load a modified binary if the module being loaded
	// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
	// to intercept the required .NET methods, if any, and call the real LoadLibrary
	// on the modified binary if there were intercepts.  Doing it here in the caller
	// functions prevents the modified binary path from showing up in the log, so that
	// the user sees the original module path.
	if (!CLRAllowModuleLoad(*((SiString *)params[0])))
	{
		// CLRAllowModuleLoad returned false, meaning that the module was the native
		// form of a .NET binary that we are intercepting.  By denying LoadLibrary on
		// this module, we can force the .NET runtime to recompile the intercepted
		// module, allowing our interception to take place
		*retValue = 0;
		*errCode = ERROR_FILE_NOT_FOUND;
		return true;
	}

	SiString replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
	HMODULE temp;

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, LoadLibrary should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
	{
		temp = realLoadLibraryExA(*((SiString *)params[0]), *((HANDLE *)params[1]),
			*((DWORD *)params[2]));
	}
	else
	{
		temp = realLoadLibraryExA(replacementBinary, *((HANDLE *)params[1]),
			*((DWORD *)params[2]));
	}

	memcpy(retValue, &temp, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LoadLibraryExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LoadLibraryExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;

	// .NET interception: We need to load a modified binary if the module being loaded
	// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
	// to intercept the required .NET methods, if any, and call the real LoadLibrary
	// on the modified binary if there were intercepts.  Doing it here in the caller
	// functions prevents the modified binary path from showing up in the log, so that
	// the user sees the original module path.
	if (!CLRAllowModuleLoad(*((SiString *)params[0])))
	{
		// CLRAllowModuleLoad returned false, meaning that the module was the native
		// form of a .NET binary that we are intercepting.  By denying LoadLibrary on
		// this module, we can force the .NET runtime to recompile the intercepted
		// module, allowing our interception to take place
		*retValue = 0;
		*errCode = ERROR_FILE_NOT_FOUND;
		return true;
	}

	SiString replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
	HMODULE temp;

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, LoadLibrary should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
	{
		temp = realLoadLibraryExW(*((SiString *)params[0]), *((HANDLE *)params[1]),
			*((DWORD *)params[2]));
	}
	else
	{
		temp = realLoadLibraryExW(replacementBinary, *((HANDLE *)params[1]),
			*((DWORD *)params[2]));
	}

	memcpy(retValue, &temp, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LoadLibraryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LoadLibraryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;

	// .NET interception: We need to load a modified binary if the module being loaded
	// has .NET methods that need to be intercepted.  We will call CLRInterceptModule
	// to intercept the required .NET methods, if any, and call the real LoadLibrary
	// on the modified binary if there were intercepts.  Doing it here in the caller
	// functions prevents the modified binary path from showing up in the log, so that
	// the user sees the original module path.
	if (!CLRAllowModuleLoad(*((SiString *)params[0])))
	{
		// CLRAllowModuleLoad returned false, meaning that the module was the native
		// form of a .NET binary that we are intercepting.  By denying LoadLibrary on
		// this module, we can force the .NET runtime to recompile the intercepted
		// module, allowing our interception to take place
		*retValue = 0;
		*errCode = ERROR_FILE_NOT_FOUND;
		return true;
	}

	SiString replacementBinary = CLRCheckInterceptedCache(*((SiString *)params[0]));
	HMODULE temp;

	// CLRInterceptModule returns an empty string if the original module should be used.
	// If a path string was returned, LoadLibrary should be called on the returned path
	// instead, as the new path contains the intercepted module.
	if (replacementBinary == "")
		temp = realLoadLibraryW(*((SiString *)params[0]));
	else
		temp = realLoadLibraryW(replacementBinary);

	memcpy(retValue, &temp, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LocalAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LocalAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HLOCAL tempRetValue = realLocalAlloc(*((UINT *)params[0]), *((SIZE_T *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LocalFreeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LocalFreeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HLOCAL tempRetValue = realLocalFree(*((HLOCAL *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LocalReAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LocalReAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	HLOCAL tempRetValue = realLocalReAlloc(*((HLOCAL *)params[0]), *((SIZE_T *)params[1]), *((UINT *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MapUserPhysicalPagesCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MapUserPhysicalPagesCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realMapUserPhysicalPages(*((PVOID *)params[0]), *((ULONG_PTR *)params[1]), *((PULONG_PTR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MapUserPhysicalPagesScatterCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MapUserPhysicalPagesScatterCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realMapUserPhysicalPagesScatter(*((PVOID * *)params[0]), *((ULONG_PTR *)params[1]), *((PULONG_PTR *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MapViewOfFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MapViewOfFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	LPVOID tempRetValue = realMapViewOfFile(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((SIZE_T *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MapViewOfFileExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MapViewOfFileExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	LPVOID tempRetValue = realMapViewOfFileEx(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((SIZE_T *)params[4]), *((LPVOID *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realMoveFileA(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realMoveFileExA(*((SiString *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realMoveFileExW(*((SiString *)params[0]), *((SiString *)params[1]), *((DWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realMoveFileW(*((SiString *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileWithProgressACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileWithProgressACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realMoveFileWithProgressA(*((SiString *)params[0]), *((SiString *)params[1]), *((LPPROGRESS_ROUTINE *)params[2]), *((LPVOID *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MoveFileWithProgressWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MoveFileWithProgressWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realMoveFileWithProgressW(*((SiString *)params[0]), *((SiString *)params[1]), *((LPPROGRESS_ROUTINE *)params[2]), *((LPVOID *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ReadFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ReadFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = NetCorrupt_ReadFile(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPOVERLAPPED *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ReadFileExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ReadFileExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = NetCorrupt_ReadFileEx(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((DWORD *)params[2]), *((LPOVERLAPPED *)params[3]), *((LPOVERLAPPED_COMPLETION_ROUTINE *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ReadFileScatterCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ReadFileScatterCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = NetCorrupt_ReadFileScatter(*((HANDLE *)params[0]), *((FILE_SEGMENT_ELEMENT **)params[1]),
		*((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPOVERLAPPED *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RemoveDirectoryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RemoveDirectoryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realRemoveDirectoryA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: RemoveDirectoryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool RemoveDirectoryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realRemoveDirectoryW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ReplaceFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ReplaceFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realReplaceFileA(*((SiString *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((DWORD *)params[3]), *((LPVOID *)params[4]), *((LPVOID *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ReplaceFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ReplaceFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realReplaceFileW(*((SiString *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((DWORD *)params[3]), *((LPVOID *)params[4]), *((LPVOID *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SearchPathACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SearchPathACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	DWORD tempRetValue = realSearchPathA(*((SiString *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((DWORD *)params[3]), *((LPSTR *)params[4]), *((LPSTR * *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SearchPathWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SearchPathWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	DWORD tempRetValue = realSearchPathW(*((SiString *)params[0]), *((SiString *)params[1]), *((SiString *)params[2]), *((DWORD *)params[3]), *((LPWSTR *)params[4]), *((LPWSTR * *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileAttributesACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileAttributesACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetFileAttributesA(*((SiString *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileAttributesWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileAttributesWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetFileAttributesW(*((SiString *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	LPVOID tempRetValue = realVirtualAlloc(*((LPVOID *)params[0]), *((SIZE_T *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualAllocExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualAllocExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	LPVOID tempRetValue = realVirtualAllocEx(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((SIZE_T *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualFreeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualFreeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realVirtualFree(*((LPVOID *)params[0]), *((SIZE_T *)params[1]), *((DWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualFreeExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualFreeExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realVirtualFreeEx(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((SIZE_T *)params[2]), *((DWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualLockCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualLockCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realVirtualLock(*((LPVOID *)params[0]), *((SIZE_T *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualQueryCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualQueryCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	SIZE_T tempRetValue = realVirtualQuery(*((LPCVOID *)params[0]), *((PMEMORY_BASIC_INFORMATION *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualQueryExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualQueryExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	SIZE_T tempRetValue = realVirtualQueryEx(*((HANDLE *)params[0]), *((LPCVOID *)params[1]), *((PMEMORY_BASIC_INFORMATION *)params[2]), *((SIZE_T *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualUnlockCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualUnlockCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realVirtualUnlock(*((LPVOID *)params[0]), *((SIZE_T *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: WriteFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WriteFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = NetCorrupt_WriteFile(*((HANDLE *)params[0]), *((LPCVOID *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPOVERLAPPED *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: WriteFileExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WriteFileExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = NetCorrupt_WriteFileEx(*((HANDLE *)params[0]), *((LPCVOID *)params[1]), *((DWORD *)params[2]), *((LPOVERLAPPED *)params[3]), *((LPOVERLAPPED_COMPLETION_ROUTINE *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: WriteFileGatherCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WriteFileGatherCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	*retValue = NetCorrupt_WriteFileGather(*((HANDLE *)params[0]), *((FILE_SEGMENT_ELEMENT **)params[1]), 
		*((DWORD *)params[2]), *((LPDWORD *)params[3]), 
		*((LPOVERLAPPED *)params[4]));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: AllocConsoleCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool AllocConsoleCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	BOOL tempRetValue = realAllocConsole();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: AreFileApisANSICaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool AreFileApisANSICaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	BOOL tempRetValue = realAreFileApisANSI();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: AttachConsoleCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool AttachConsoleCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realAttachConsole(*((DWORD *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: BackupReadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool BackupReadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	BOOL tempRetValue = realBackupRead(*((HANDLE *)params[0]), *((LPBYTE *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((BOOL *)params[4]), *((BOOL *)params[5]), *((LPVOID * *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: BackupSeekCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool BackupSeekCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realBackupSeek(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPVOID * *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: BackupWriteCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool BackupWriteCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	BOOL tempRetValue = realBackupWrite(*((HANDLE *)params[0]), *((LPBYTE *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((BOOL *)params[4]), *((BOOL *)params[5]), *((LPVOID * *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CancelIoCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CancelIoCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = NetCorrupt_CancelIo(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CloseHandleCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CloseHandleCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = NetCorrupt_CloseHandle(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFiberCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFiberCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	LPVOID tempRetValue = realCreateFiber(*((SIZE_T *)params[0]), *((LPFIBER_START_ROUTINE *)params[1]), *((LPVOID *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFiberExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFiberExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	LPVOID tempRetValue = realCreateFiberEx(*((SIZE_T *)params[0]), *((SIZE_T *)params[1]), *((DWORD *)params[2]), *((LPFIBER_START_ROUTINE *)params[3]), *((LPVOID *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFileMappingACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFileMappingACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realCreateFileMappingA(*((HANDLE *)params[0]), *((LPSECURITY_ATTRIBUTES *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]), *((SiString *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateFileMappingWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateFileMappingWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realCreateFileMappingW(*((HANDLE *)params[0]), *((LPSECURITY_ATTRIBUTES *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]), *((SiString *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateHardLinkACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateHardLinkACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCreateHardLinkA(*((SiString *)params[0]), *((SiString *)params[1]), *((LPSECURITY_ATTRIBUTES *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateHardLinkWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateHardLinkWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realCreateHardLinkW(*((SiString *)params[0]), *((SiString *)params[1]), *((LPSECURITY_ATTRIBUTES *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateMemoryResourceNotificationCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateMemoryResourceNotificationCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HANDLE tempRetValue = realCreateMemoryResourceNotification(*((MEMORY_RESOURCE_NOTIFICATION_TYPE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateThreadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateThreadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realCreateThread(*((LPSECURITY_ATTRIBUTES *)params[0]), *((SIZE_T *)params[1]), *((LPTHREAD_START_ROUTINE *)params[2]), *((LPVOID *)params[3]), *((DWORD *)params[4]), *((LPDWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateToolhelp32SnapshotCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateToolhelp32SnapshotCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realCreateToolhelp32Snapshot(*((DWORD *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: ContinueDebugEventCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ContinueDebugEventCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realContinueDebugEvent(*((DWORD *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DebugActiveProcessCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DebugActiveProcessCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDebugActiveProcess(*((DWORD *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DebugActiveProcessStopCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DebugActiveProcessStopCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDebugActiveProcessStop(*((DWORD *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DebugBreakCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DebugBreakCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	realDebugBreak();
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DebugBreakProcessCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DebugBreakProcessCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDebugBreakProcess(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DebugSetProcessKillOnExitCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DebugSetProcessKillOnExitCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDebugSetProcessKillOnExit(*((BOOL *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FatalAppExitACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FatalAppExitACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	realFatalAppExitA(*((UINT *)params[0]), *((SiString *)params[1]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FatalAppExitWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FatalAppExitWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	realFatalAppExitW(*((UINT *)params[0]), *((SiString *)params[1]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FatalExitCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FatalExitCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realFatalExit(*((int *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DeleteFiberCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DeleteFiberCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realDeleteFiber(*((LPVOID *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DisableThreadLibraryCallsCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DisableThreadLibraryCallsCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realDisableThreadLibraryCalls(*((HMODULE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DnsHostnameToComputerNameACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DnsHostnameToComputerNameACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realDnsHostnameToComputerNameA(*((SiString *)params[0]), *((SiString *)params[1]), *((LPDWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DnsHostnameToComputerNameWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DnsHostnameToComputerNameWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realDnsHostnameToComputerNameW(*((SiString *)params[0]), *((SiString *)params[1]), *((LPDWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindFirstFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindFirstFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realFindFirstFileA(*((SiString *)params[0]), *((LPWIN32_FIND_DATAA *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindFirstFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindFirstFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	HANDLE tempRetValue = realFindFirstFileW(*((SiString *)params[0]), *((LPWIN32_FIND_DATAW *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindFirstFileExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindFirstFileExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realFindFirstFileExA(*((SiString *)params[0]), *((FINDEX_INFO_LEVELS *)params[1]), *((LPVOID *)params[2]), *((FINDEX_SEARCH_OPS *)params[3]), *((LPVOID *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindFirstFileExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindFirstFileExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realFindFirstFileExW(*((SiString *)params[0]), *((FINDEX_INFO_LEVELS *)params[1]), *((LPVOID *)params[2]), *((FINDEX_SEARCH_OPS *)params[3]), *((LPVOID *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindNextFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindNextFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFindNextFileA(*((HANDLE *)params[0]), *((LPWIN32_FIND_DATAA *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FindNextFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FindNextFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFindNextFileW(*((HANDLE *)params[0]), *((LPWIN32_FIND_DATAW *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetCompressedFileSizeACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetCompressedFileSizeACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetCompressedFileSizeA(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetCompressedFileSizeWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetCompressedFileSizeWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetCompressedFileSizeW(*((SiString *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetExitCodeProcessCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetExitCodeProcessCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetExitCodeProcess(*((HANDLE *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetExitCodeThreadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetExitCodeThreadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetExitCodeThread(*((HANDLE *)params[0]), *((LPDWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFullPathNameACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFullPathNameACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	DWORD tempRetValue = realGetFullPathNameA(*((SiString *)params[0]), *((DWORD *)params[1]), *((LPSTR *)params[2]), *((LPSTR * *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFullPathNameWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFullPathNameWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	DWORD tempRetValue = realGetFullPathNameW(*((SiString *)params[0]), *((DWORD *)params[1]), *((LPWSTR *)params[2]), *((LPWSTR * *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcessHeapCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcessHeapCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	HANDLE tempRetValue = realGetProcessHeap();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcessHeapsCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcessHeapsCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realGetProcessHeaps(*((DWORD *)params[0]), *((PHANDLE *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcessIoCountersCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcessIoCountersCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realGetProcessIoCounters(*((HANDLE *)params[0]), *((PIO_COUNTERS *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcessTimesCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcessTimesCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realGetProcessTimes(*((HANDLE *)params[0]), *((LPFILETIME *)params[1]), *((LPFILETIME *)params[2]), *((LPFILETIME *)params[3]), *((LPFILETIME *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcessWorkingSetSizeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcessWorkingSetSizeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realGetProcessWorkingSetSize(*((HANDLE *)params[0]), *((PSIZE_T *)params[1]), *((PSIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetStartupInfoACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetStartupInfoACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetStartupInfoA(*((LPSTARTUPINFOA *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetStartupInfoWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetStartupInfoWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetStartupInfoW(*((LPSTARTUPINFOW *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemDirectoryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemDirectoryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	UINT tempRetValue = realGetSystemDirectoryA(*((LPSTR *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemDirectoryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemDirectoryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	UINT tempRetValue = realGetSystemDirectoryW(*((LPWSTR *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemWindowsDirectoryACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemWindowsDirectoryACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	UINT tempRetValue = realGetSystemWindowsDirectoryA(*((LPSTR *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemWindowsDirectoryWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemWindowsDirectoryWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	UINT tempRetValue = realGetSystemWindowsDirectoryW(*((LPWSTR *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetWriteWatchCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetWriteWatchCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	UINT tempRetValue = realGetWriteWatch(*((DWORD *)params[0]), *((PVOID *)params[1]), *((SIZE_T *)params[2]), *((PVOID * *)params[3]), *((PULONG_PTR *)params[4]), *((PULONG *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GlobalMemoryStatusCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GlobalMemoryStatusCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGlobalMemoryStatus(*((LPMEMORYSTATUS *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GlobalReAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GlobalReAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	HGLOBAL tempRetValue = realGlobalReAlloc(*((HGLOBAL *)params[0]), *((SIZE_T *)params[1]), *((UINT *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LockFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LockFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realLockFile(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LockFileExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LockFileExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	BOOL tempRetValue = realLockFileEx(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]), *((LPOVERLAPPED *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZCloseCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZCloseCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realLZClose(*((INT *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZCopyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZCopyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	realLZCopy(*((INT *)params[0]), *((INT *)params[1]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZOpenFileACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZOpenFileACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	INT tempRetValue = realLZOpenFileA(*((SiString *)params[0]), *((LPOFSTRUCT *)params[1]), *((WORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZOpenFileWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZOpenFileWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	INT tempRetValue = realLZOpenFileW(*((SiString *)params[0]), *((LPOFSTRUCT *)params[1]), *((WORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZReadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZReadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	INT tempRetValue = realLZRead(*((INT *)params[0]), *((LPSTR *)params[1]), *((INT *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LZSeekCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LZSeekCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	LONG tempRetValue = realLZSeek(*((INT *)params[0]), *((LONG *)params[1]), *((INT *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LoadModuleCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LoadModuleCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	DWORD tempRetValue = realLoadModule(*((SiString *)params[0]), *((LPVOID *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: OpenFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool OpenFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	HFILE tempRetValue = realOpenFile(*((SiString *)params[0]), *((LPOFSTRUCT *)params[1]), *((UINT *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: QueryMemoryResourceNotificationCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool QueryMemoryResourceNotificationCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realQueryMemoryResourceNotification(*((HANDLE *)params[0]), *((PBOOL *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileApisToANSICaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileApisToANSICaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	realSetFileApisToANSI();
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileApisToOEMCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileApisToOEMCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	realSetFileApisToOEM();
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFilePointerCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFilePointerCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	DWORD tempRetValue = realSetFilePointer(*((HANDLE *)params[0]), *((LONG *)params[1]), *((PLONG *)params[2]), *((DWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFilePointerExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFilePointerExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realSetFilePointerEx(*((HANDLE *)params[0]), *((LARGE_INTEGER *)params[1]), *((PLARGE_INTEGER *)params[2]), *((DWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileShortNameACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileShortNameACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetFileShortNameA(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileShortNameWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileShortNameWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetFileShortNameW(*((HANDLE *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileValidDataCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileValidDataCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetFileValidData(*((HANDLE *)params[0]), *((LONGLONG *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetEndOfFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetEndOfFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realSetEndOfFile(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetPriorityClassCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetPriorityClassCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetPriorityClass(*((HANDLE *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetProcessShutdownParametersCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetProcessShutdownParametersCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetProcessShutdownParameters(*((DWORD *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetProcessWorkingSetSizeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetProcessWorkingSetSizeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realSetProcessWorkingSetSize(*((HANDLE *)params[0]), *((SIZE_T *)params[1]), *((SIZE_T *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: TerminateProcessCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool TerminateProcessCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realTerminateProcess(*((HANDLE *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: TerminateThreadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool TerminateThreadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realTerminateThread(*((HANDLE *)params[0]), *((DWORD *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: TlsAllocCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool TlsAllocCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	DWORD tempRetValue = realTlsAlloc();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: TlsFreeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool TlsFreeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realTlsFree(*((DWORD *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: UnlockFileCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool UnlockFileCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realUnlockFile(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: UnlockFileExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool UnlockFileExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realUnlockFileEx(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((DWORD *)params[2]), *((DWORD *)params[3]), *((LPOVERLAPPED *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualProtectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualProtectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realVirtualProtect(*((LPVOID *)params[0]), *((SIZE_T *)params[1]), *((DWORD *)params[2]), *((PDWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: VirtualProtectExCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool VirtualProtectExCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realVirtualProtectEx(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((SIZE_T *)params[2]), *((DWORD *)params[3]), *((PDWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: WriteProcessMemoryCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WriteProcessMemoryCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = realWriteProcessMemory(*((HANDLE *)params[0]), *((LPVOID *)params[1]), *((LPCVOID *)params[2]), *((SIZE_T *)params[3]), *((SIZE_T * *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CompareFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CompareFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	LONG tempRetValue = realCompareFileTime(*((CONST FILETIME * *)params[0]), *((CONST FILETIME * *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DosDateTimeToFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DosDateTimeToFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realDosDateTimeToFileTime(*((WORD *)params[0]), *((WORD *)params[1]), *((LPFILETIME *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FileTimeToDosDateTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FileTimeToDosDateTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realFileTimeToDosDateTime(*((CONST FILETIME * *)params[0]), *((LPWORD *)params[1]), *((LPWORD *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FileTimeToLocalFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FileTimeToLocalFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFileTimeToLocalFileTime(*((CONST FILETIME * *)params[0]), *((LPFILETIME *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: FileTimeToSystemTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool FileTimeToSystemTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realFileTimeToSystemTime(*((CONST FILETIME * *)params[0]), *((LPSYSTEMTIME *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realGetFileTime(*((HANDLE *)params[0]), *((LPFILETIME *)params[1]), *((LPFILETIME *)params[2]), *((LPFILETIME *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetLocalTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetLocalTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetLocalTime(*((LPSYSTEMTIME *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetSystemTime(*((LPSYSTEMTIME *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemTimeAdjustmentCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemTimeAdjustmentCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realGetSystemTimeAdjustment(*((PDWORD *)params[0]), *((PDWORD *)params[1]), *((PBOOL *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemTimeAsFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemTimeAsFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetSystemTimeAsFileTime(*((LPFILETIME *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetTimeFormatACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetTimeFormatACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	int tempRetValue = realGetTimeFormatA(*((LCID *)params[0]), *((DWORD *)params[1]), *((CONST SYSTEMTIME * *)params[2]), *((SiString *)params[3]), *((LPSTR *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetTimeFormatWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetTimeFormatWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	int tempRetValue = realGetTimeFormatW(*((LCID *)params[0]), *((DWORD *)params[1]), *((CONST SYSTEMTIME * *)params[2]), *((SiString *)params[3]), *((LPWSTR *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetTimeZoneInformationCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetTimeZoneInformationCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	DWORD tempRetValue = realGetTimeZoneInformation(*((LPTIME_ZONE_INFORMATION *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LocalFileTimeToFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LocalFileTimeToFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realLocalFileTimeToFileTime(*((CONST FILETIME * *)params[0]), *((LPFILETIME *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realSetFileTime(*((HANDLE *)params[0]), *((CONST FILETIME * *)params[1]), *((CONST FILETIME * *)params[2]), *((CONST FILETIME * *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetLocalTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetLocalTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realSetLocalTime(*((CONST SYSTEMTIME * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetSystemTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetSystemTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realSetSystemTime(*((CONST SYSTEMTIME * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetSystemTimeAdjustmentCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetSystemTimeAdjustmentCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSetSystemTimeAdjustment(*((DWORD *)params[0]), *((BOOL *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SetTimeZoneInformationCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SetTimeZoneInformationCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realSetTimeZoneInformation(*((CONST TIME_ZONE_INFORMATION * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SystemTimeToFileTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SystemTimeToFileTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	BOOL tempRetValue = realSystemTimeToFileTime(*((CONST SYSTEMTIME * *)params[0]), *((LPFILETIME *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: SystemTimeToTzSpecificLocalTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool SystemTimeToTzSpecificLocalTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realSystemTimeToTzSpecificLocalTime(*((LPTIME_ZONE_INFORMATION *)params[0]), *((LPSYSTEMTIME *)params[1]), *((LPSYSTEMTIME *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: TzSpecificLocalTimeToSystemTimeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool TzSpecificLocalTimeToSystemTimeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	BOOL tempRetValue = realTzSpecificLocalTimeToSystemTime(*((LPTIME_ZONE_INFORMATION *)params[0]), *((LPSYSTEMTIME *)params[1]), *((LPSYSTEMTIME *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: QueryPerformanceCounterCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool QueryPerformanceCounterCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realQueryPerformanceCounter(*((LARGE_INTEGER * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: QueryPerformanceFrequencyCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool QueryPerformanceFrequencyCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realQueryPerformanceFrequency(*((LARGE_INTEGER * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetProcAddressCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetProcAddressCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;

	DWORD lpProcNameAddr = *((DWORD *)params[1]);
	if (lpProcNameAddr & 0xFFFF0000)
	{
		FARPROC tempRetValue = realGetProcAddress(*((HMODULE *)params[0]), *((SiString *)params[1]));
		memcpy (retValue, &tempRetValue, sizeof(DWORD));
		*errCode = GetLastError();
		return true;
	}
	else
	{
		FARPROC tempRetValue = realGetProcAddress(*((HMODULE *)params[0]), *((LPCSTR *)params[1]));
		memcpy (retValue, &tempRetValue, sizeof(DWORD));
		*errCode = GetLastError();
		return true;
	}
}
//*************************************************************************
// Method: GetModuleHandleACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetModuleHandleACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HMODULE tempRetValue = realGetModuleHandleA(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetModuleHandleWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetModuleHandleWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	HMODULE tempRetValue = realGetModuleHandleW(*((SiString *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DeviceIoControlCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DeviceIoControlCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	BOOL tempRetValue = realDeviceIoControl(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((LPVOID *)params[2]), *((DWORD *)params[3]), *((LPVOID *)params[4]), *((DWORD *)params[5]), *((LPDWORD *)params[6]), *((LPOVERLAPPED *)params[7]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetVersionExACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetVersionExACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realGetVersionExA(*((LPOSVERSIONINFOA *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetVersionExWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetVersionExWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	BOOL tempRetValue = realGetVersionExW(*((LPOSVERSIONINFOW *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetSystemInfoCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetSystemInfoCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realGetSystemInfo(*((LPSYSTEM_INFO *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetVolumeInformationACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetVolumeInformationACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	BOOL tempRetValue = realGetVolumeInformationA(*((SiString *)params[0]), *((LPSTR *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPDWORD *)params[5]), *((LPSTR *)params[6]), *((DWORD *)params[7]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetVolumeInformationWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetVolumeInformationWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	BOOL tempRetValue = realGetVolumeInformationW(*((SiString *)params[0]), *((LPWSTR *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPDWORD *)params[5]), *((LPWSTR *)params[6]), *((DWORD *)params[7]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateRemoteThreadCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateRemoteThreadCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	HANDLE tempRetValue = realCreateRemoteThread(*((HANDLE *)params[0]), *((LPSECURITY_ATTRIBUTES *)params[1]), *((SIZE_T *)params[2]), *((LPTHREAD_START_ROUTINE *)params[3]), *((LPVOID *)params[4]), *((DWORD *)params[5]), *((LPDWORD *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcpyACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcpyACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	LPSTR tempRetValue = reallstrcpyA(*((LPSTR *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcatACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcatACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	LPSTR tempRetValue = reallstrcatA(*((LPSTR *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcpynACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcpynACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	LPSTR tempRetValue = reallstrcpynA(*((LPSTR *)params[0]), *((SiString *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: MultiByteToWideCharCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool MultiByteToWideCharCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;

	int tempRetValue;
	if (*((int *)params[3]) == -1)
	{
		// Null terminated string, parameter passed as SiString
		tempRetValue = realMultiByteToWideChar(*((UINT *)params[0]), *((DWORD *)params[1]), *((SiString *)params[2]), *((int *)params[3]), *((LPWSTR *)params[4]), *((int *)params[5]));
	}
	else
	{
		// String not null terminated, parameter passed as pointer
		tempRetValue = realMultiByteToWideChar(*((UINT *)params[0]), *((DWORD *)params[1]), *((LPCSTR *)params[2]), *((int *)params[3]), *((LPWSTR *)params[4]), *((int *)params[5]));
	}

	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: WinExecCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WinExecCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	UINT tempRetValue = realWinExec(*((SiString *)params[0]), *((UINT *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: InitializeCriticalSectionCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool InitializeCriticalSectionCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realInitializeCriticalSection(*((LPCRITICAL_SECTION *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: EnterCriticalSectionCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool EnterCriticalSectionCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realEnterCriticalSection(*((LPCRITICAL_SECTION *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcpyWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcpyWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	LPWSTR tempRetValue = reallstrcpyW(*((LPWSTR *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcatWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcatWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	LPWSTR tempRetValue = reallstrcatW(*((LPWSTR *)params[0]), *((SiString *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: lstrcpynWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool lstrcpynWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	LPWSTR tempRetValue = reallstrcpynW(*((LPWSTR *)params[0]), *((SiString *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: LeaveCriticalSectionCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool LeaveCriticalSectionCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realLeaveCriticalSection(*((LPCRITICAL_SECTION *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: DeleteCriticalSectionCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool DeleteCriticalSectionCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	realDeleteCriticalSection(*((LPCRITICAL_SECTION *)params[0]));
	*retValue = 0;
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: CreateIoCompletionPortCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool CreateIoCompletionPortCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	HANDLE tempRetValue = NetCorrupt_CreateIoCompletionPort(*((HANDLE *)params[0]), *((HANDLE *)params[1]), *((ULONG_PTR *)params[2]), *((DWORD *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: GetQueuedCompletionStatusCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool GetQueuedCompletionStatusCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	BOOL tempRetValue = NetCorrupt_GetQueuedCompletionStatus(*((HANDLE *)params[0]), *((LPDWORD *)params[1]), *((PULONG_PTR *)params[2]), *((LPOVERLAPPED * *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = GetLastError();
	return true;
}
//*************************************************************************
// Method: PostQueuedCompletionStatusCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool PostQueuedCompletionStatusCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	BOOL tempRetValue = realPostQueuedCompletionStatus(*((HANDLE *)params[0]), *((DWORD *)params[1]), *((ULONG_PTR *)params[2]), *((LPOVERLAPPED *)params[3]));
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
// Method:		AllocateUserPhysicalPagesReplacement
// Description: See MSDN AllocateUserPhysicalPages function
// Parameters:	See MSDN AllocateUserPhysicalPages parameters
// Return Value: See MSDN AllocateUserPhysicalPages return value
//*************************************************************************
BOOL WINAPI AllocateUserPhysicalPagesReplacement(HANDLE hProcess, PULONG_PTR NumberOfPages, PULONG_PTR PageArray)
{
	const int numParams = 3;
	char *functionName = "AllocateUserPhysicalPages";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &NumberOfPages, &PageArray };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realAllocateUserPhysicalPages == NULL)
		realAllocateUserPhysicalPages = (AllocateUserPhysicalPagesFunction)library->GetOriginalFunction(functionName);
		
	if (realAllocateUserPhysicalPages != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		SYSTEM_INFO systemInfo;
		DWORD pageSize, allocInfo = 0;

		GetSystemInfo(&systemInfo);
		pageSize = systemInfo.dwPageSize;
		if (NumberOfPages != NULL)
			allocInfo = (*NumberOfPages) * pageSize;
		
		if (library->RunStandardTestsAndGetResults(logSender, AllocateUserPhysicalPagesCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CopyFileAReplacement
// Description: See MSDN CopyFileA function
// Parameters:	See MSDN CopyFileA parameters
// Return Value: See MSDN CopyFileA return value
//*************************************************************************
BOOL WINAPI CopyFileAReplacement(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
{
	const int numParams = 3;
	char *functionName = "CopyFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpExistingFileName, (char *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &bFailIfExists };
	ParameterType paramTypes[numParams] = { StringType, StringType, BooleanType };
	
	if (realCopyFileA == NULL)
		realCopyFileA = (CopyFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realCopyFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfo = 0, allocInfoTarget = 0;
		bool positiveAllocation = true;
		
		allocInfo = library->GetFileSizeInfoA(lpExistingFileName);
		if (!bFailIfExists)
		{
			allocInfoTarget = library->GetFileSizeInfoA(lpNewFileName);
			positiveAllocation = allocInfo >= allocInfoTarget;
			if (positiveAllocation)
				allocInfo = allocInfo - allocInfoTarget;
			else
				allocInfo = allocInfoTarget - allocInfo;

		}
		
		if (library->RunStandardTestsAndGetResults(logSender, CopyFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CopyFileExAReplacement
// Description: See MSDN CopyFileExA function
// Parameters:	See MSDN CopyFileExA parameters
// Return Value: See MSDN CopyFileExA return value
//*************************************************************************
BOOL WINAPI CopyFileExAReplacement(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags)
{
	const int numParams = 6;
	char *functionName = "CopyFileExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpExistingFileName, (char *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &lpProgressRoutine, &lpData, &pbCancel, &dwCopyFlags };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realCopyFileExA == NULL)
		realCopyFileExA = (CopyFileExAFunction)library->GetOriginalFunction(functionName);
		
	if (realCopyFileExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfo = 0, allocInfoTarget = 0;
		bool positiveAllocation = true;

		allocInfo = library->GetFileSizeInfoA(lpExistingFileName);
		if ( (dwCopyFlags & COPY_FILE_FAIL_IF_EXISTS) != COPY_FILE_FAIL_IF_EXISTS)
		{
			allocInfoTarget = library->GetFileSizeInfoA(lpNewFileName);
			positiveAllocation = allocInfo >= allocInfoTarget;
			if (positiveAllocation)
				allocInfo = allocInfo - allocInfoTarget;
			else
				allocInfo = allocInfoTarget - allocInfo;
		}
		
		if (library->RunStandardTestsAndGetResults(logSender, CopyFileExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CopyFileExWReplacement
// Description: See MSDN CopyFileExW function
// Parameters:	See MSDN CopyFileExW parameters
// Return Value: See MSDN CopyFileExW return value
//*************************************************************************
BOOL WINAPI CopyFileExWReplacement(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags)
{
	const int numParams = 6;
	char *functionName = "CopyFileExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &lpProgressRoutine, &lpData, &pbCancel, &dwCopyFlags };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realCopyFileExW == NULL)
		realCopyFileExW = (CopyFileExWFunction)library->GetOriginalFunction(functionName);
		
	if (realCopyFileExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfo = 0, allocInfoTarget = 0;
		bool positiveAllocation = true;

		allocInfo = library->GetFileSizeInfoW(lpExistingFileName);
		if ( (dwCopyFlags & COPY_FILE_FAIL_IF_EXISTS) != COPY_FILE_FAIL_IF_EXISTS)
		{
			allocInfoTarget = library->GetFileSizeInfoW(lpNewFileName);
			positiveAllocation = allocInfo >= allocInfoTarget;
			if (positiveAllocation)
				allocInfo = allocInfo - allocInfoTarget;
			else
				allocInfo = allocInfoTarget - allocInfo;
		}
		
		if (library->RunStandardTestsAndGetResults(logSender, CopyFileExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CopyFileWReplacement
// Description: See MSDN CopyFileW function
// Parameters:	See MSDN CopyFileW parameters
// Return Value: See MSDN CopyFileW return value
//*************************************************************************
BOOL WINAPI CopyFileWReplacement(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)
{
	const int numParams = 3;
	char *functionName = "CopyFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &bFailIfExists };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, BooleanType };
	
	if (realCopyFileW == NULL)
		realCopyFileW = (CopyFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realCopyFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfo = 0, allocInfoTarget = 0;
		bool positiveAllocation = true;
		
		allocInfo = library->GetFileSizeInfoW(lpExistingFileName);
		if (!bFailIfExists)
		{
			allocInfoTarget = library->GetFileSizeInfoW(lpNewFileName);
			positiveAllocation = allocInfo >= allocInfoTarget;
			if (positiveAllocation)
				allocInfo = allocInfo - allocInfoTarget;
			else
				allocInfo = allocInfoTarget - allocInfo;
		}
		
		//if the function was unchanged (return val and errorcode untouched), then update USED counter
		
		if (library->RunStandardTestsAndGetResults(logSender, CopyFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CreateDirectoryAReplacement
// Description: See MSDN CreateDirectoryA function
// Parameters:	See MSDN CreateDirectoryA parameters
// Return Value: See MSDN CreateDirectoryA return value
//*************************************************************************
BOOL WINAPI CreateDirectoryAReplacement(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 2;
	char *functionName = "CreateDirectoryA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpPathName };
	void *params[numParams] = { &str[0], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realCreateDirectoryA == NULL)
		realCreateDirectoryA = (CreateDirectoryAFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateDirectoryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateDirectoryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateDirectoryExAReplacement
// Description: See MSDN CreateDirectoryExA function
// Parameters:	See MSDN CreateDirectoryExA parameters
// Return Value: See MSDN CreateDirectoryExA return value
//*************************************************************************
BOOL WINAPI CreateDirectoryExAReplacement(LPCSTR lpTemplateDirectory, LPCSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "CreateDirectoryExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpTemplateDirectory, (char *)lpNewDirectory };
	void *params[numParams] = { &str[0], &str[1], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType };
	
	if (realCreateDirectoryExA == NULL)
		realCreateDirectoryExA = (CreateDirectoryExAFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateDirectoryExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateDirectoryExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateDirectoryExWReplacement
// Description: See MSDN CreateDirectoryExW function
// Parameters:	See MSDN CreateDirectoryExW parameters
// Return Value: See MSDN CreateDirectoryExW return value
//*************************************************************************
BOOL WINAPI CreateDirectoryExWReplacement(LPCWSTR lpTemplateDirectory, LPCWSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "CreateDirectoryExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpTemplateDirectory, (wchar_t *)lpNewDirectory };
	void *params[numParams] = { &str[0], &str[1], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType };
	
	if (realCreateDirectoryExW == NULL)
		realCreateDirectoryExW = (CreateDirectoryExWFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateDirectoryExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateDirectoryExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateDirectoryWReplacement
// Description: See MSDN CreateDirectoryW function
// Parameters:	See MSDN CreateDirectoryW parameters
// Return Value: See MSDN CreateDirectoryW return value
//*************************************************************************
BOOL WINAPI CreateDirectoryWReplacement(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 2;
	char *functionName = "CreateDirectoryW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpPathName };
	void *params[numParams] = { &str[0], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realCreateDirectoryW == NULL)
		realCreateDirectoryW = (CreateDirectoryWFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateDirectoryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateDirectoryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateFileAReplacement
// Description: See MSDN CreateFileA function
// Parameters:	See MSDN CreateFileA parameters
// Return Value: See MSDN CreateFileA return value
//*************************************************************************
HANDLE WINAPI CreateFileAReplacement(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttibutes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	const int numParams = 7;
	char *functionName = "CreateFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &dwDesiredAccess, &dwShareMode, &lpSecurityAttibutes, &dwCreationDisposition, &dwFlagsAndAttributes, &hTemplateFile };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType, UnsignedLongType, PointerType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realCreateFileA == NULL)
		realCreateFileA = (CreateFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		unsigned long allocInfo = 0;
		bool positiveAllocation = true;
		
		switch (dwCreationDisposition)
		{
		case CREATE_NEW:
			allocInfo = 1;
			break;
		case CREATE_ALWAYS:
			allocInfo = library->GetFileSizeInfoA(lpFileName);
			positiveAllocation = false;
			break;
		case OPEN_EXISTING:
			allocInfo = 0;
			break;
		case OPEN_ALWAYS:
			allocInfo = library->GetFileSizeInfoA(lpFileName);
			if (allocInfo = 0)
				allocInfo = 1;
			else
				positiveAllocation = false;
			break;
		case TRUNCATE_EXISTING:
			allocInfo = library->GetFileSizeInfoA(lpFileName);
			positiveAllocation = false;
			break;
		}
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		library->CreateResourceMapping(returnValue, lpFileName);
		
		if (returnValue != INVALID_HANDLE_VALUE)
			library->UpdateDiskUsage (allocInfo, positiveAllocation);
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		CreateFileWReplacement
// Description: See MSDN CreateFileW function
// Parameters:	See MSDN CreateFileW parameters
// Return Value: See MSDN CreateFileW return value
//*************************************************************************
HANDLE WINAPI CreateFileWReplacement(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttibutes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	const int numParams = 7;
	char *functionName = "CreateFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &dwDesiredAccess, &dwShareMode, &lpSecurityAttibutes, &dwCreationDisposition, &dwFlagsAndAttributes, &hTemplateFile };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType, UnsignedLongType, PointerType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realCreateFileW == NULL)
		realCreateFileW = (CreateFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		unsigned long allocInfo = 0;
		bool positiveAllocation = true;
		
		switch (dwCreationDisposition)
		{
		case CREATE_NEW:
			allocInfo = 1;
			break;
		case CREATE_ALWAYS:
			allocInfo = library->GetFileSizeInfoW(lpFileName);
			positiveAllocation = false;
			break;
		case OPEN_EXISTING:
			allocInfo = 0;
			break;
		case OPEN_ALWAYS:
			allocInfo = library->GetFileSizeInfoW(lpFileName);
			if (allocInfo = 0)
				allocInfo = 1;
			else
				positiveAllocation = false;
			break;
		case TRUNCATE_EXISTING:
			allocInfo = library->GetFileSizeInfoW(lpFileName);
			positiveAllocation = false;
			break;
		}
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		library->CreateResourceMapping(returnValue, lpFileName);

		//check to see if the function succeeded
		if (returnValue != INVALID_HANDLE_VALUE)
			library->UpdateDiskUsage(allocInfo, positiveAllocation);
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
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

	const int numParams = 10;
	char *functionName = "CreateProcessA";
	char *categoryName = "PROCESS";
	SiString str[] = { (char *)lpApplicationName, (char *)lpCommandLine, (char *)lpCurrentDirectory };
	void *params[numParams] = { &str[0], &str[1], &lpProcessAttributes, &lpThreadAttributes, &bInheritHandles, &dwCreationFlags, &lpEnvironment, &str[2], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType, PointerType, BooleanType, UnsignedLongType, PointerType, StringType, PointerType, PointerType };

	if (realCreateProcessA == NULL)
		realCreateProcessA = (CreateProcessAFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessA != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
		{
			// tell holodeck the pid
			DWORD pid = lpProcessInformation->dwProcessId;
			char pidString[12];
			_ultoa(pid, pidString, 10);
			SiString fileString = NEW_PROCESS_FLAG;
			fileString += " ";
			fileString += pidString;
			fileString += " ";
			// write the resume app flag
			fileString += ((dwCreationFlags & CREATE_SUSPENDED) ? "0" : "1");
			childProcessDataFile->Write(fileString, strlen(fileString) + 1);

			// tell holodeck the process was created
			childProcessCreatedEvent->Signal();

			// wait for holodeck to be done processing
			holodeckAttachedToChildProcessEvent->Acquire();
		}

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

	const int numParams = 10;
	char *functionName = "CreateProcessW";
	char *categoryName = "PROCESS";
	SiString str[] = { (wchar_t *)lpApplicationName, (wchar_t *)lpCommandLine, (wchar_t *)lpCurrentDirectory };
	void *params[numParams] = { &str[0], &str[1], &lpProcessAttributes, &lpThreadAttributes, &bInheritHandles, &dwCreationFlags, &lpEnvironment, &str[2], &lpStartupInfo, &lpProcessInformation };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType, PointerType, BooleanType, UnsignedLongType, PointerType, WideStringType, PointerType, PointerType };

	if (realCreateProcessW == NULL)
		realCreateProcessW = (CreateProcessWFunction)library->GetOriginalFunction(functionName);

	if (realCreateProcessW != NULL)
	{
		DWORD errorCode, returnValue;
		library->RunStandardTestsAndGetResults(logSender, CreateProcessWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, 0, true);
		SetLastError(errorCode);

		if (lpProcessInformation)
		{
			// tell holodeck the pid
			DWORD pid = lpProcessInformation->dwProcessId;
			char pidString[12];
			_ultoa(pid, pidString, 10);

			SiString fileString = NEW_PROCESS_FLAG;
			fileString += " ";
			fileString += pidString;
			fileString += " ";
			// write the resume app flag
			fileString += ((dwCreationFlags & CREATE_SUSPENDED) ? "0" : "1");
			childProcessDataFile->Write(fileString, strlen(fileString) + 1);

			// tell holodeck the process was created
			childProcessCreatedEvent->Signal();

			// wait for holodeck to be done processing
			holodeckAttachedToChildProcessEvent->Acquire();
		}

		createProcessMutex->Release();
		return returnValue;
	}

	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		DeleteFileAReplacement
// Description: See MSDN DeleteFileA function
// Parameters:	See MSDN DeleteFileA parameters
// Return Value: See MSDN DeleteFileA return value
//*************************************************************************
BOOL WINAPI DeleteFileAReplacement(LPCSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "DeleteFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realDeleteFileA == NULL)
		realDeleteFileA = (DeleteFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realDeleteFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//Get size information before it can be deleted\lost.
		unsigned long allocInfo = library->GetFileSizeInfoA (lpFileName);
		
		if (library->RunStandardTestsAndGetResults(logSender, DeleteFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, false))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, false);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		DeleteFileWReplacement
// Description: See MSDN DeleteFileW function
// Parameters:	See MSDN DeleteFileW parameters
// Return Value: See MSDN DeleteFileW return value
//*************************************************************************
BOOL WINAPI DeleteFileWReplacement(LPCWSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "DeleteFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realDeleteFileW == NULL)
		realDeleteFileW = (DeleteFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realDeleteFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//Get size information before it can be deleted\lost.
		unsigned long allocInfo = library->GetFileSizeInfoW (lpFileName);
		
		if (library->RunStandardTestsAndGetResults(logSender, DeleteFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, false))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, false);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		FlushFileBuffersReplacement
// Description: See MSDN FlushFileBuffers function
// Parameters:	See MSDN FlushFileBuffers parameters
// Return Value: See MSDN FlushFileBuffers return value
//*************************************************************************
BOOL WINAPI FlushFileBuffersReplacement(HANDLE hFile)
{
	const int numParams = 1;
	char *functionName = "FlushFileBuffers";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realFlushFileBuffers == NULL)
		realFlushFileBuffers = (FlushFileBuffersFunction)library->GetOriginalFunction(functionName);
		
	if (realFlushFileBuffers != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, FlushFileBuffersCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FreeLibraryReplacement
// Description: See MSDN FreeLibrary function
// Parameters:	See MSDN FreeLibrary parameters
// Return Value: See MSDN FreeLibrary return value
//*************************************************************************
BOOL WINAPI FreeLibraryReplacement(HMODULE hLibModule)
{
	const int numParams = 1;
	char *functionName = "FreeLibrary";
	char *categoryName = "LIBRARY";
	
	void *params[numParams] = { &hLibModule };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realFreeLibrary == NULL)
		realFreeLibrary = (FreeLibraryFunction)library->GetOriginalFunction(functionName);
		
	if (realFreeLibrary != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FreeLibraryCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FreeLibraryAndExitThreadReplacement
// Description: See MSDN FreeLibraryAndExitThread function
// Parameters:	See MSDN FreeLibraryAndExitThread parameters
// Return Value: See MSDN FreeLibraryAndExitThread return value
//*************************************************************************
VOID WINAPI FreeLibraryAndExitThreadReplacement(HMODULE hLibModule, DWORD dwExitCode)
{
	const int numParams = 2;
	char *functionName = "FreeLibraryAndExitThread";
	char *categoryName = "LIBRARY";
	
	void *params[numParams] = { &hLibModule, &dwExitCode };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realFreeLibraryAndExitThread == NULL)
		realFreeLibraryAndExitThread = (FreeLibraryAndExitThreadFunction)library->GetOriginalFunction(functionName);
		
	if (realFreeLibraryAndExitThread != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FreeLibraryAndExitThreadCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		FreeUserPhysicalPagesReplacement
// Description: See MSDN FreeUserPhysicalPages function
// Parameters:	See MSDN FreeUserPhysicalPages parameters
// Return Value: See MSDN FreeUserPhysicalPages return value
//*************************************************************************
BOOL WINAPI FreeUserPhysicalPagesReplacement(HANDLE hProcess, PULONG_PTR NumberOfPages, PULONG_PTR PageArray)
{
	const int numParams = 3;
	char *functionName = "FreeUserPhysicalPages";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &NumberOfPages, &PageArray };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realFreeUserPhysicalPages == NULL)
		realFreeUserPhysicalPages = (FreeUserPhysicalPagesFunction)library->GetOriginalFunction(functionName);
		
	if (realFreeUserPhysicalPages != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FreeUserPhysicalPagesCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetBinaryTypeAReplacement
// Description: See MSDN GetBinaryTypeA function
// Parameters:	See MSDN GetBinaryTypeA parameters
// Return Value: See MSDN GetBinaryTypeA return value
//*************************************************************************
BOOL WINAPI GetBinaryTypeAReplacement(LPCSTR lpApplicationName, LPDWORD lpBinaryType)
{
	const int numParams = 2;
	char *functionName = "GetBinaryTypeA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpApplicationName };
	void *params[numParams] = { &str[0], &lpBinaryType };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realGetBinaryTypeA == NULL)
		realGetBinaryTypeA = (GetBinaryTypeAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetBinaryTypeA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetBinaryTypeACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetBinaryTypeWReplacement
// Description: See MSDN GetBinaryTypeW function
// Parameters:	See MSDN GetBinaryTypeW parameters
// Return Value: See MSDN GetBinaryTypeW return value
//*************************************************************************
BOOL WINAPI GetBinaryTypeWReplacement(LPCWSTR lpApplicationName, LPDWORD lpBinaryType)
{
	const int numParams = 2;
	char *functionName = "GetBinaryTypeW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpApplicationName };
	void *params[numParams] = { &str[0], &lpBinaryType };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realGetBinaryTypeW == NULL)
		realGetBinaryTypeW = (GetBinaryTypeWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetBinaryTypeW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetBinaryTypeWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetCurrentDirectoryAReplacement
// Description: See MSDN GetCurrentDirectoryA function
// Parameters:	See MSDN GetCurrentDirectoryA parameters
// Return Value: See MSDN GetCurrentDirectoryA return value
//*************************************************************************
DWORD WINAPI GetCurrentDirectoryAReplacement(DWORD nBufferLength, LPSTR lpBuffer)
{
	const int numParams = 2;
	char *functionName = "GetCurrentDirectoryA";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &nBufferLength, &lpBuffer };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType };
	
	if (realGetCurrentDirectoryA == NULL)
		realGetCurrentDirectoryA = (GetCurrentDirectoryAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetCurrentDirectoryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetCurrentDirectoryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetCurrentDirectoryWReplacement
// Description: See MSDN GetCurrentDirectoryW function
// Parameters:	See MSDN GetCurrentDirectoryW parameters
// Return Value: See MSDN GetCurrentDirectoryW return value
//*************************************************************************
DWORD WINAPI GetCurrentDirectoryWReplacement(DWORD nBufferLength, LPWSTR lpBuffer)
{
	const int numParams = 2;
	char *functionName = "GetCurrentDirectoryW";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &nBufferLength, &lpBuffer };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType };
	
	if (realGetCurrentDirectoryW == NULL)
		realGetCurrentDirectoryW = (GetCurrentDirectoryWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetCurrentDirectoryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetCurrentDirectoryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetDiskFreeSpaceAReplacement
// Description: See MSDN GetDiskFreeSpaceA function
// Parameters:	See MSDN GetDiskFreeSpaceA parameters
// Return Value: See MSDN GetDiskFreeSpaceA return value
//*************************************************************************
BOOL WINAPI GetDiskFreeSpaceAReplacement(LPCSTR lpRootPathName, LPDWORD lpSectorsPerCluster, LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters, LPDWORD lpTotalNumberOfClusters)
{
	const int numParams = 5;
	char *functionName = "GetDiskFreeSpaceA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpRootPathName };
	void *params[numParams] = { &str[0], &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters };
	ParameterType paramTypes[numParams] = { StringType, PointerType, PointerType, PointerType, PointerType };
	
	if (realGetDiskFreeSpaceA == NULL)
		realGetDiskFreeSpaceA = (GetDiskFreeSpaceAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDiskFreeSpaceA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDiskFreeSpaceACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//If disk limits are set, then change param values to reflect user set limit
		DWORD64 maxDiskSpace = 0;
		DWORD64 currDiskUsage = 0;
		DWORD64 bytesAvailable = 0;
		if (library->GetMaxDiskInfo(maxDiskSpace))
		{
			library->GetCurrentDiskUsage(currDiskUsage);
			if (currDiskUsage > maxDiskSpace)
				bytesAvailable = 0;
			else
				bytesAvailable = maxDiskSpace - currDiskUsage;

			*lpNumberOfFreeClusters = (bytesAvailable) / ((*lpSectorsPerCluster) * (*lpBytesPerSector));
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetDiskFreeSpaceExAReplacement
// Description: See MSDN GetDiskFreeSpaceExA function
// Parameters:	See MSDN GetDiskFreeSpaceExA parameters
// Return Value: See MSDN GetDiskFreeSpaceExA return value
//*************************************************************************
BOOL WINAPI GetDiskFreeSpaceExAReplacement(LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes)
{
	const int numParams = 4;
	char *functionName = "GetDiskFreeSpaceExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpDirectoryName };
	void *params[numParams] = { &str[0], &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes };
	ParameterType paramTypes[numParams] = { StringType, PointerType, PointerType, PointerType };
	
	if (realGetDiskFreeSpaceExA == NULL)
		realGetDiskFreeSpaceExA = (GetDiskFreeSpaceExAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDiskFreeSpaceExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDiskFreeSpaceExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//If disk limits are set, then change param values to reflect user set limit
		DWORD64 maxDiskSpace = 0;
		DWORD64 currDiskUsage = 0;
		DWORD64 bytesAvailable = 0;
		if (library->GetMaxDiskInfo(maxDiskSpace))
		{
			library->GetCurrentDiskUsage(currDiskUsage);
			if (currDiskUsage > maxDiskSpace)
				bytesAvailable = 0;
			else
				bytesAvailable = maxDiskSpace - currDiskUsage;

			//lpTotalNumberOfBytes->QuadPart = maxDiskSpace; //we do not want to change this
			if (lpFreeBytesAvailableToCaller)
				lpFreeBytesAvailableToCaller->QuadPart = bytesAvailable;
			if (lpTotalNumberOfFreeBytes)
				lpTotalNumberOfFreeBytes->QuadPart = bytesAvailable;
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetDiskFreeSpaceExWReplacement
// Description: See MSDN GetDiskFreeSpaceExW function
// Parameters:	See MSDN GetDiskFreeSpaceExW parameters
// Return Value: See MSDN GetDiskFreeSpaceExW return value
//*************************************************************************
BOOL WINAPI GetDiskFreeSpaceExWReplacement(LPCWSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes)
{
	const int numParams = 4;
	char *functionName = "GetDiskFreeSpaceExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpDirectoryName };
	void *params[numParams] = { &str[0], &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, PointerType, PointerType };
	
	if (realGetDiskFreeSpaceExW == NULL)
		realGetDiskFreeSpaceExW = (GetDiskFreeSpaceExWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDiskFreeSpaceExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDiskFreeSpaceExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//If disk limits are set, then change param values to reflect user set limit
		DWORD64 maxDiskSpace = 0;
		DWORD64 currDiskUsage = 0;
		DWORD64 bytesAvailable = 0;
		if (library->GetMaxDiskInfo(maxDiskSpace))
		{
			library->GetCurrentDiskUsage(currDiskUsage);
			if (currDiskUsage > maxDiskSpace)
				bytesAvailable = 0;
			else
				bytesAvailable = maxDiskSpace - currDiskUsage;

			//lpTotalNumberOfBytes->QuadPart = maxDiskSpace; //we do not want to change this
			if (lpFreeBytesAvailableToCaller)
				lpFreeBytesAvailableToCaller->QuadPart = bytesAvailable;
			if (lpTotalNumberOfFreeBytes)
				lpTotalNumberOfFreeBytes->QuadPart = bytesAvailable;
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetDiskFreeSpaceWReplacement
// Description: See MSDN GetDiskFreeSpaceW function
// Parameters:	See MSDN GetDiskFreeSpaceW parameters
// Return Value: See MSDN GetDiskFreeSpaceW return value
//*************************************************************************
BOOL WINAPI GetDiskFreeSpaceWReplacement(LPCWSTR lpRootPathName, LPDWORD lpSectorsPerCluster, LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters, LPDWORD lpTotalNumberOfClusters)
{
	const int numParams = 5;
	char *functionName = "GetDiskFreeSpaceW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpRootPathName };
	void *params[numParams] = { &str[0], &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, PointerType, PointerType, PointerType };
	
	if (realGetDiskFreeSpaceW == NULL)
		realGetDiskFreeSpaceW = (GetDiskFreeSpaceWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDiskFreeSpaceW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDiskFreeSpaceWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//If disk limits are set, then change param values to reflect user set limit
		DWORD64 maxDiskSpace = 0;
		DWORD64 currDiskUsage = 0;
		DWORD64 bytesAvailable = 0;
		if (library->GetMaxDiskInfo(maxDiskSpace))
		{
			library->GetCurrentDiskUsage(currDiskUsage);
			if (currDiskUsage > maxDiskSpace)
				bytesAvailable = 0;
			else
				bytesAvailable = maxDiskSpace - currDiskUsage;

			*lpNumberOfFreeClusters = (bytesAvailable) / ((*lpSectorsPerCluster) * (*lpBytesPerSector));
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GetDriveTypeAReplacement
// Description: See MSDN GetDriveTypeA function
// Parameters:	See MSDN GetDriveTypeA parameters
// Return Value: See MSDN GetDriveTypeA return value
//*************************************************************************
UINT WINAPI GetDriveTypeAReplacement(LPCSTR lpRootPathName)
{
	const int numParams = 1;
	char *functionName = "GetDriveTypeA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpRootPathName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realGetDriveTypeA == NULL)
		realGetDriveTypeA = (GetDriveTypeAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDriveTypeA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDriveTypeACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetDriveTypeWReplacement
// Description: See MSDN GetDriveTypeW function
// Parameters:	See MSDN GetDriveTypeW parameters
// Return Value: See MSDN GetDriveTypeW return value
//*************************************************************************
UINT WINAPI GetDriveTypeWReplacement(LPCWSTR lpRootPathName)
{
	const int numParams = 1;
	char *functionName = "GetDriveTypeW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpRootPathName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realGetDriveTypeW == NULL)
		realGetDriveTypeW = (GetDriveTypeWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetDriveTypeW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetDriveTypeWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetFileAttributesAReplacement
// Description: See MSDN GetFileAttributesA function
// Parameters:	See MSDN GetFileAttributesA parameters
// Return Value: See MSDN GetFileAttributesA return value
//*************************************************************************
DWORD WINAPI GetFileAttributesAReplacement(LPCSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "GetFileAttributesA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realGetFileAttributesA == NULL)
		realGetFileAttributesA = (GetFileAttributesAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileAttributesA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileAttributesACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetFileAttributesExAReplacement
// Description: See MSDN GetFileAttributesExA function
// Parameters:	See MSDN GetFileAttributesExA parameters
// Return Value: See MSDN GetFileAttributesExA return value
//*************************************************************************
BOOL WINAPI GetFileAttributesExAReplacement(LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
{
	const int numParams = 3;
	char *functionName = "GetFileAttributesExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &fInfoLevelId, &lpFileInformation };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType, PointerType };
	
	if (realGetFileAttributesExA == NULL)
		realGetFileAttributesExA = (GetFileAttributesExAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileAttributesExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileAttributesExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFileAttributesExWReplacement
// Description: See MSDN GetFileAttributesExW function
// Parameters:	See MSDN GetFileAttributesExW parameters
// Return Value: See MSDN GetFileAttributesExW return value
//*************************************************************************
BOOL WINAPI GetFileAttributesExWReplacement(LPCWSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
{
	const int numParams = 3;
	char *functionName = "GetFileAttributesExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &fInfoLevelId, &lpFileInformation };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType, PointerType };
	
	if (realGetFileAttributesExW == NULL)
		realGetFileAttributesExW = (GetFileAttributesExWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileAttributesExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileAttributesExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFileAttributesWReplacement
// Description: See MSDN GetFileAttributesW function
// Parameters:	See MSDN GetFileAttributesW parameters
// Return Value: See MSDN GetFileAttributesW return value
//*************************************************************************
DWORD WINAPI GetFileAttributesWReplacement(LPCWSTR lpFileName)
{
	const int numParams = 1;
	char *functionName = "GetFileAttributesW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realGetFileAttributesW == NULL)
		realGetFileAttributesW = (GetFileAttributesWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileAttributesW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileAttributesWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetFileInformationByHandleReplacement
// Description: See MSDN GetFileInformationByHandle function
// Parameters:	See MSDN GetFileInformationByHandle parameters
// Return Value: See MSDN GetFileInformationByHandle return value
//*************************************************************************
BOOL WINAPI GetFileInformationByHandleReplacement(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
	const int numParams = 2;
	char *functionName = "GetFileInformationByHandle";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpFileInformation };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetFileInformationByHandle == NULL)
		realGetFileInformationByHandle = (GetFileInformationByHandleFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileInformationByHandle != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileInformationByHandleCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFileSizeReplacement
// Description: See MSDN GetFileSize function
// Parameters:	See MSDN GetFileSize parameters
// Return Value: See MSDN GetFileSize return value
//*************************************************************************
DWORD WINAPI GetFileSizeReplacement(HANDLE hFile, LPDWORD lpFileSizeHigh)
{
	const int numParams = 2;
	char *functionName = "GetFileSize";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpFileSizeHigh };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetFileSize == NULL)
		realGetFileSize = (GetFileSizeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileSize != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileSizeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetFileSizeExReplacement
// Description: See MSDN GetFileSizeEx function
// Parameters:	See MSDN GetFileSizeEx parameters
// Return Value: See MSDN GetFileSizeEx return value
//*************************************************************************
BOOL WINAPI GetFileSizeExReplacement(HANDLE hFile, PLARGE_INTEGER lpFileSize)
{
	const int numParams = 2;
	char *functionName = "GetFileSizeEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpFileSize };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetFileSizeEx == NULL)
		realGetFileSizeEx = (GetFileSizeExFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileSizeEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileSizeExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFileTypeReplacement
// Description: See MSDN GetFileType function
// Parameters:	See MSDN GetFileType parameters
// Return Value: See MSDN GetFileType return value
//*************************************************************************
DWORD WINAPI GetFileTypeReplacement(HANDLE hFile)
{
	const int numParams = 1;
	char *functionName = "GetFileType";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetFileType == NULL)
		realGetFileType = (GetFileTypeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileType != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileTypeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetLogicalDrivesReplacement
// Description: See MSDN GetLogicalDrives function
// Parameters:	See MSDN GetLogicalDrives parameters
// Return Value: See MSDN GetLogicalDrives return value
//*************************************************************************
DWORD WINAPI GetLogicalDrivesReplacement()
{
	const int numParams = 0;
	char *functionName = "GetLogicalDrives";
	char *categoryName = "FILE";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realGetLogicalDrives == NULL)
		realGetLogicalDrives = (GetLogicalDrivesFunction)library->GetOriginalFunction(functionName);
		
	if (realGetLogicalDrives != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetLogicalDrivesCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetLogicalDriveStringsAReplacement
// Description: See MSDN GetLogicalDriveStringsA function
// Parameters:	See MSDN GetLogicalDriveStringsA parameters
// Return Value: See MSDN GetLogicalDriveStringsA return value
//*************************************************************************
DWORD WINAPI GetLogicalDriveStringsAReplacement(DWORD nBufferLength, LPSTR lpBuffer)
{
	const int numParams = 2;
	char *functionName = "GetLogicalDriveStringsA";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &nBufferLength, &lpBuffer };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType };
	
	if (realGetLogicalDriveStringsA == NULL)
		realGetLogicalDriveStringsA = (GetLogicalDriveStringsAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetLogicalDriveStringsA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetLogicalDriveStringsACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetLogicalDriveStringsWReplacement
// Description: See MSDN GetLogicalDriveStringsW function
// Parameters:	See MSDN GetLogicalDriveStringsW parameters
// Return Value: See MSDN GetLogicalDriveStringsW return value
//*************************************************************************
DWORD WINAPI GetLogicalDriveStringsWReplacement(DWORD nBufferLength, LPWSTR lpBuffer)
{
	const int numParams = 2;
	char *functionName = "GetLogicalDriveStringsW";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &nBufferLength, &lpBuffer };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType };
	
	if (realGetLogicalDriveStringsW == NULL)
		realGetLogicalDriveStringsW = (GetLogicalDriveStringsWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetLogicalDriveStringsW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetLogicalDriveStringsWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GlobalAllocReplacement
// Description: See MSDN GlobalAlloc function
// Parameters:	See MSDN GlobalAlloc parameters
// Return Value: See MSDN GlobalAlloc return value
//*************************************************************************
HGLOBAL WINAPI GlobalAllocReplacement(UINT uFlags, SIZE_T dwBytes)
{
	const int numParams = 2;
	char *functionName = "GlobalAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &uFlags, &dwBytes };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType };
	
	if (realGlobalAlloc == NULL)
		realGlobalAlloc = (GlobalAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realGlobalAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HGLOBAL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GlobalAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HGLOBAL", &errorCode, (unsigned long)dwBytes, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		GlobalFreeReplacement
// Description: See MSDN GlobalFree function
// Parameters:	See MSDN GlobalFree parameters
// Return Value: See MSDN GlobalFree return value
//*************************************************************************
HGLOBAL WINAPI GlobalFreeReplacement(HGLOBAL hMem)
{
	const int numParams = 1;
	char *functionName = "GlobalFree";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hMem };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGlobalFree == NULL)
		realGlobalFree = (GlobalFreeFunction)library->GetOriginalFunction(functionName);
		
	if (realGlobalFree != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HGLOBAL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GlobalFreeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HGLOBAL", &errorCode, 0, true))
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
// Method:		GlobalMemoryStatusExReplacement
// Description: See MSDN GlobalMemoryStatusEx function
// Parameters:	See MSDN GlobalMemoryStatusEx parameters
// Return Value: See MSDN GlobalMemoryStatusEx return value
//*************************************************************************
BOOL WINAPI GlobalMemoryStatusExReplacement(LPMEMORYSTATUSEX lpBuffer)
{
	const int numParams = 1;
	char *functionName = "GlobalMemoryStatusEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpBuffer };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGlobalMemoryStatusEx == NULL)
		realGlobalMemoryStatusEx = (GlobalMemoryStatusExFunction)library->GetOriginalFunction(functionName);
		
	if (realGlobalMemoryStatusEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GlobalMemoryStatusExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		//If memory limits are set, then change param values to reflect user set limit
		DWORD64 maxMemSpace = 0;
		DWORD64 currMemUsage = 0;
		if (library->GetMaxMemInfo(maxMemSpace))
		{
			library->GetCurrentMemUsage(currMemUsage);
			lpBuffer->ullAvailPageFile = maxMemSpace - currMemUsage;
			lpBuffer->ullAvailPhys = maxMemSpace - currMemUsage;
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		HeapAllocReplacement
// Description: See MSDN HeapAlloc function
// Parameters:	See MSDN HeapAlloc parameters
// Return Value: See MSDN HeapAlloc return value
//*************************************************************************
LPVOID WINAPI HeapAllocReplacement(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
	const int numParams = 3;
	char *functionName = "HeapAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags, &dwBytes };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realHeapAlloc == NULL)
		realHeapAlloc = (HeapAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, (unsigned long)dwBytes, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		HeapCompactReplacement
// Description: See MSDN HeapCompact function
// Parameters:	See MSDN HeapCompact parameters
// Return Value: See MSDN HeapCompact return value
//*************************************************************************
SIZE_T WINAPI HeapCompactReplacement(HANDLE hHeap, DWORD dwFlags)
{
	const int numParams = 2;
	char *functionName = "HeapCompact";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realHeapCompact == NULL)
		realHeapCompact = (HeapCompactFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapCompact != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapCompactCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		HeapCreateReplacement
// Description: See MSDN HeapCreate function
// Parameters:	See MSDN HeapCreate parameters
// Return Value: See MSDN HeapCreate return value
//*************************************************************************
HANDLE WINAPI HeapCreateReplacement(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
	const int numParams = 3;
	char *functionName = "HeapCreate";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &flOptions, &dwInitialSize, &dwMaximumSize };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realHeapCreate == NULL)
		realHeapCreate = (HeapCreateFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapCreate != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapCreateCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, (unsigned long)dwInitialSize, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		HeapDestroyReplacement
// Description: See MSDN HeapDestroy function
// Parameters:	See MSDN HeapDestroy parameters
// Return Value: See MSDN HeapDestroy return value
//*************************************************************************
BOOL WINAPI HeapDestroyReplacement(HANDLE hHeap)
{
	const int numParams = 1;
	char *functionName = "HeapDestroy";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realHeapDestroy == NULL)
		realHeapDestroy = (HeapDestroyFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapDestroy != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapDestroyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		HeapFreeReplacement
// Description: See MSDN HeapFree function
// Parameters:	See MSDN HeapFree parameters
// Return Value: See MSDN HeapFree return value
//*************************************************************************
BOOL WINAPI HeapFreeReplacement(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
	const int numParams = 3;
	char *functionName = "HeapFree";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags, &lpMem };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };
	
	if (realHeapFree == NULL)
		realHeapFree = (HeapFreeFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapFree != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapFreeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		HeapLockReplacement
// Description: See MSDN HeapLock function
// Parameters:	See MSDN HeapLock parameters
// Return Value: See MSDN HeapLock return value
//*************************************************************************
BOOL WINAPI HeapLockReplacement(HANDLE hHeap)
{
	const int numParams = 1;
	char *functionName = "HeapLock";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realHeapLock == NULL)
		realHeapLock = (HeapLockFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapLock != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapLockCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		HeapReAllocReplacement
// Description: See MSDN HeapReAlloc function
// Parameters:	See MSDN HeapReAlloc parameters
// Return Value: See MSDN HeapReAlloc return value
//*************************************************************************
LPVOID WINAPI HeapReAllocReplacement(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes)
{
	const int numParams = 4;
	char *functionName = "HeapReAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags, &lpMem, &dwBytes };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, UnsignedLongType };
	
	if (realHeapReAlloc == NULL)
		realHeapReAlloc = (HeapReAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapReAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		//bugbug - should walk the heap to get size of original heap before realloc, but worried about side-effects.
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapReAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, (unsigned long)dwBytes, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		HeapSizeReplacement
// Description: See MSDN HeapSize function
// Parameters:	See MSDN HeapSize parameters
// Return Value: See MSDN HeapSize return value
//*************************************************************************
SIZE_T WINAPI HeapSizeReplacement(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
{
	const int numParams = 3;
	char *functionName = "HeapSize";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags, &lpMem };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };
	
	if (realHeapSize == NULL)
		realHeapSize = (HeapSizeFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapSize != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapSizeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		HeapUnlockReplacement
// Description: See MSDN HeapUnlock function
// Parameters:	See MSDN HeapUnlock parameters
// Return Value: See MSDN HeapUnlock return value
//*************************************************************************
BOOL WINAPI HeapUnlockReplacement(HANDLE hHeap)
{
	const int numParams = 1;
	char *functionName = "HeapUnlock";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realHeapUnlock == NULL)
		realHeapUnlock = (HeapUnlockFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapUnlock != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapUnlockCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		HeapValidateReplacement
// Description: See MSDN HeapValidate function
// Parameters:	See MSDN HeapValidate parameters
// Return Value: See MSDN HeapValidate return value
//*************************************************************************
BOOL WINAPI HeapValidateReplacement(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem)
{
	const int numParams = 3;
	char *functionName = "HeapValidate";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hHeap, &dwFlags, &lpMem };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };
	
	if (realHeapValidate == NULL)
		realHeapValidate = (HeapValidateFunction)library->GetOriginalFunction(functionName);
		
	if (realHeapValidate != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, HeapValidateCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		LoadLibraryAReplacement
// Description: See MSDN LoadLibraryA function
// Parameters:	See MSDN LoadLibraryA parameters
// Return Value: See MSDN LoadLibraryA return value
//*************************************************************************
HMODULE WINAPI LoadLibraryAReplacement(LPCSTR lpLibFileName)
{
	const int numParams = 1;
	char *functionName = "LoadLibraryA";
	char *categoryName = "LIBRARY";
	SiString str[] = { (char *)lpLibFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realLoadLibraryA == NULL)
		realLoadLibraryA = (LoadLibraryAFunction)library->GetOriginalFunction(functionName);
		
	if (realLoadLibraryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LoadLibraryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		LoadLibraryExAReplacement
// Description: See MSDN LoadLibraryExA function
// Parameters:	See MSDN LoadLibraryExA parameters
// Return Value: See MSDN LoadLibraryExA return value
//*************************************************************************
HMODULE WINAPI LoadLibraryExAReplacement(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "LoadLibraryExA";
	char *categoryName = "LIBRARY";
	SiString str[] = { (char *)lpLibFileName };
	void *params[numParams] = { &str[0], &hFile, &dwFlags };
	ParameterType paramTypes[numParams] = { StringType, PointerType, UnsignedLongType };
	
	if (realLoadLibraryExA == NULL)
		realLoadLibraryExA = (LoadLibraryExAFunction)library->GetOriginalFunction(functionName);
		
	if (realLoadLibraryExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LoadLibraryExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		LoadLibraryExWReplacement
// Description: See MSDN LoadLibraryExW function
// Parameters:	See MSDN LoadLibraryExW parameters
// Return Value: See MSDN LoadLibraryExW return value
//*************************************************************************
HMODULE WINAPI LoadLibraryExWReplacement(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "LoadLibraryExW";
	char *categoryName = "LIBRARY";
	SiString str[] = { (wchar_t *)lpLibFileName };
	void *params[numParams] = { &str[0], &hFile, &dwFlags };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, UnsignedLongType };
	
	if (realLoadLibraryExW == NULL)
		realLoadLibraryExW = (LoadLibraryExWFunction)library->GetOriginalFunction(functionName);
		
	if (realLoadLibraryExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LoadLibraryExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		LoadLibraryWReplacement
// Description: See MSDN LoadLibraryW function
// Parameters:	See MSDN LoadLibraryW parameters
// Return Value: See MSDN LoadLibraryW return value
//*************************************************************************
HMODULE WINAPI LoadLibraryWReplacement(LPCWSTR lpLibFileName)
{
	const int numParams = 1;
	char *functionName = "LoadLibraryW";
	char *categoryName = "LIBRARY";
	SiString str[] = { (wchar_t *)lpLibFileName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realLoadLibraryW == NULL)
		realLoadLibraryW = (LoadLibraryWFunction)library->GetOriginalFunction(functionName);
		
	if (realLoadLibraryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LoadLibraryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		LocalAllocReplacement
// Description: See MSDN LocalAlloc function
// Parameters:	See MSDN LocalAlloc parameters
// Return Value: See MSDN LocalAlloc return value
//*************************************************************************
HLOCAL WINAPI LocalAllocReplacement(UINT uFlags, SIZE_T uBytes)
{
	const int numParams = 2;
	char *functionName = "LocalAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &uFlags, &uBytes };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType };
	
	if (realLocalAlloc == NULL)
		realLocalAlloc = (LocalAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realLocalAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HLOCAL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LocalAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HLOCAL", &errorCode, (unsigned long)uBytes, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		LocalFreeReplacement
// Description: See MSDN LocalFree function
// Parameters:	See MSDN LocalFree parameters
// Return Value: See MSDN LocalFree return value
//*************************************************************************
HLOCAL WINAPI LocalFreeReplacement(HLOCAL hMem)
{
	const int numParams = 1;
	char *functionName = "LocalFree";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hMem };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realLocalFree == NULL)
		realLocalFree = (LocalFreeFunction)library->GetOriginalFunction(functionName);
		
	if (realLocalFree != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HLOCAL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LocalFreeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HLOCAL", &errorCode, 0, true))
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
// Method:		LocalReAllocReplacement
// Description: See MSDN LocalReAlloc function
// Parameters:	See MSDN LocalReAlloc parameters
// Return Value: See MSDN LocalReAlloc return value
//*************************************************************************
HLOCAL WINAPI LocalReAllocReplacement(HLOCAL hMem, SIZE_T uBytes, UINT uFlags)
{
	const int numParams = 3;
	char *functionName = "LocalReAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hMem, &uBytes, &uFlags };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realLocalReAlloc == NULL)
		realLocalReAlloc = (LocalReAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realLocalReAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HLOCAL returnValue;
		UINT allocationInfo = LocalSize(hMem);
		bool positiveAllocation = uBytes >= allocationInfo;

		if (positiveAllocation)
			allocationInfo = uBytes - allocationInfo;
		else
			allocationInfo = allocationInfo - uBytes;
		
		if (library->RunStandardTestsAndGetResults(logSender, LocalReAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HLOCAL", &errorCode, (unsigned long)allocationInfo, positiveAllocation))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		MapUserPhysicalPagesReplacement
// Description: See MSDN MapUserPhysicalPages function
// Parameters:	See MSDN MapUserPhysicalPages parameters
// Return Value: See MSDN MapUserPhysicalPages return value
//*************************************************************************
BOOL WINAPI MapUserPhysicalPagesReplacement(PVOID VirtualAddress, ULONG_PTR NumberOfPages, PULONG_PTR PageArray)
{
	const int numParams = 3;
	char *functionName = "MapUserPhysicalPages";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &VirtualAddress, &NumberOfPages, &PageArray };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realMapUserPhysicalPages == NULL)
		realMapUserPhysicalPages = (MapUserPhysicalPagesFunction)library->GetOriginalFunction(functionName);
		
	if (realMapUserPhysicalPages != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MapUserPhysicalPagesCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		MapUserPhysicalPagesScatterReplacement
// Description: See MSDN MapUserPhysicalPagesScatter function
// Parameters:	See MSDN MapUserPhysicalPagesScatter parameters
// Return Value: See MSDN MapUserPhysicalPagesScatter return value
//*************************************************************************
BOOL WINAPI MapUserPhysicalPagesScatterReplacement(PVOID * VirtualAddress, ULONG_PTR NumberOfPages, PULONG_PTR PageArray)
{
	const int numParams = 3;
	char *functionName = "MapUserPhysicalPagesScatter";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &VirtualAddress, &NumberOfPages, &PageArray };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realMapUserPhysicalPagesScatter == NULL)
		realMapUserPhysicalPagesScatter = (MapUserPhysicalPagesScatterFunction)library->GetOriginalFunction(functionName);
		
	if (realMapUserPhysicalPagesScatter != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MapUserPhysicalPagesScatterCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		MapViewOfFileReplacement
// Description: See MSDN MapViewOfFile function
// Parameters:	See MSDN MapViewOfFile parameters
// Return Value: See MSDN MapViewOfFile return value
//*************************************************************************
LPVOID WINAPI MapViewOfFileReplacement(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap)
{
	const int numParams = 5;
	char *functionName = "MapViewOfFile";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hFileMappingObject, &dwDesiredAccess, &dwFileOffsetHigh, &dwFileOffsetLow, &dwNumberOfBytesToMap };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realMapViewOfFile == NULL)
		realMapViewOfFile = (MapViewOfFileFunction)library->GetOriginalFunction(functionName);
		
	if (realMapViewOfFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MapViewOfFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, 0, true))
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
// Method:		MapViewOfFileExReplacement
// Description: See MSDN MapViewOfFileEx function
// Parameters:	See MSDN MapViewOfFileEx parameters
// Return Value: See MSDN MapViewOfFileEx return value
//*************************************************************************
LPVOID WINAPI MapViewOfFileExReplacement(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress)
{
	const int numParams = 6;
	char *functionName = "MapViewOfFileEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hFileMappingObject, &dwDesiredAccess, &dwFileOffsetHigh, &dwFileOffsetLow, &dwNumberOfBytesToMap, &lpBaseAddress };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realMapViewOfFileEx == NULL)
		realMapViewOfFileEx = (MapViewOfFileExFunction)library->GetOriginalFunction(functionName);
		
	if (realMapViewOfFileEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MapViewOfFileExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, 0, true))
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
// Method:		MoveFileAReplacement
// Description: See MSDN MoveFileA function
// Parameters:	See MSDN MoveFileA parameters
// Return Value: See MSDN MoveFileA return value
//*************************************************************************
BOOL WINAPI MoveFileAReplacement(LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
	const int numParams = 2;
	char *functionName = "MoveFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpExistingFileName, (char *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { StringType, StringType };
	
	if (realMoveFileA == NULL)
		realMoveFileA = (MoveFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		MoveFileExAReplacement
// Description: See MSDN MoveFileExA function
// Parameters:	See MSDN MoveFileExA parameters
// Return Value: See MSDN MoveFileExA return value
//*************************************************************************
BOOL WINAPI MoveFileExAReplacement(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "MoveFileExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpExistingFileName, (char *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &dwFlags };
	ParameterType paramTypes[numParams] = { StringType, StringType, UnsignedLongType };
	
	if (realMoveFileExA == NULL)
		realMoveFileExA = (MoveFileExAFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoNewFile = library->GetFileSizeInfoA (lpNewFileName);
		bool isPositiveAllocation = false;
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfoNewFile, isPositiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfoNewFile, isPositiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		MoveFileExWReplacement
// Description: See MSDN MoveFileExW function
// Parameters:	See MSDN MoveFileExW parameters
// Return Value: See MSDN MoveFileExW return value
//*************************************************************************
BOOL WINAPI MoveFileExWReplacement(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, DWORD dwFlags)
{
	const int numParams = 3;
	char *functionName = "MoveFileExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &dwFlags };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, UnsignedLongType };
	
	if (realMoveFileExW == NULL)
		realMoveFileExW = (MoveFileExWFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoNewFile = library->GetFileSizeInfoW (lpNewFileName);
		bool isPositiveAllocation = false;
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfoNewFile, isPositiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfoNewFile, isPositiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		MoveFileWReplacement
// Description: See MSDN MoveFileW function
// Parameters:	See MSDN MoveFileW parameters
// Return Value: See MSDN MoveFileW return value
//*************************************************************************
BOOL WINAPI MoveFileWReplacement(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
	const int numParams = 2;
	char *functionName = "MoveFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1] };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType };
	
	if (realMoveFileW == NULL)
		realMoveFileW = (MoveFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		MoveFileWithProgressAReplacement
// Description: See MSDN MoveFileWithProgressA function
// Parameters:	See MSDN MoveFileWithProgressA parameters
// Return Value: See MSDN MoveFileWithProgressA return value
//*************************************************************************
BOOL WINAPI MoveFileWithProgressAReplacement(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, DWORD dwFlags)
{
	const int numParams = 5;
	char *functionName = "MoveFileWithProgressA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpExistingFileName, (char *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &lpProgressRoutine, &lpData, &dwFlags };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType, PointerType, UnsignedLongType };
	
	if (realMoveFileWithProgressA == NULL)
		realMoveFileWithProgressA = (MoveFileWithProgressAFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileWithProgressA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoNewFile = library->GetFileSizeInfoA (lpNewFileName);
		bool isPositiveAllocation = false;
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileWithProgressACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfoNewFile, isPositiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfoNewFile, isPositiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		MoveFileWithProgressWReplacement
// Description: See MSDN MoveFileWithProgressW function
// Parameters:	See MSDN MoveFileWithProgressW parameters
// Return Value: See MSDN MoveFileWithProgressW return value
//*************************************************************************
BOOL WINAPI MoveFileWithProgressWReplacement(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, DWORD dwFlags)
{
	const int numParams = 5;
	char *functionName = "MoveFileWithProgressW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	void *params[numParams] = { &str[0], &str[1], &lpProgressRoutine, &lpData, &dwFlags };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType, PointerType, UnsignedLongType };
	
	if (realMoveFileWithProgressW == NULL)
		realMoveFileWithProgressW = (MoveFileWithProgressWFunction)library->GetOriginalFunction(functionName);
		
	if (realMoveFileWithProgressW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoNewFile = library->GetFileSizeInfoW (lpNewFileName);
		bool isPositiveAllocation = false;
		
		if (library->RunStandardTestsAndGetResults(logSender, MoveFileWithProgressWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfoNewFile, isPositiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfoNewFile, isPositiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ReadFileReplacement
// Description: See MSDN ReadFile function
// Parameters:	See MSDN ReadFile parameters
// Return Value: See MSDN ReadFile return value
//*************************************************************************
BOOL WINAPI ReadFileReplacement(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 5;
	char *functionName = "ReadFile";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberOfBytesToRead, &lpNumberOfBytesRead, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realReadFile == NULL)
		realReadFile = (ReadFileFunction)library->GetOriginalFunction(functionName);
		
	if (realReadFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, ReadFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		ReadFileExReplacement
// Description: See MSDN ReadFileEx function
// Parameters:	See MSDN ReadFileEx parameters
// Return Value: See MSDN ReadFileEx return value
//*************************************************************************
BOOL WINAPI ReadFileExReplacement(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 5;
	char *functionName = "ReadFileEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberOfBytesToRead, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realReadFileEx == NULL)
		realReadFileEx = (ReadFileExFunction)library->GetOriginalFunction(functionName);
		
	if (realReadFileEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, ReadFileExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		ReadFileScatterReplacement
// Description: See MSDN ReadFileScatter function
// Parameters:	See MSDN ReadFileScatter parameters
// Return Value: See MSDN ReadFileScatter return value
//*************************************************************************
BOOL WINAPI ReadFileScatterReplacement(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[],
    DWORD nNumberOfBytesToRead, LPDWORD lpReserved, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 5;
	char *functionName = "ReadFileScatter";
	char *categoryName = "FILE";
	void *params[numParams] = { &hFile, &aSegmentArray, &nNumberOfBytesToRead, &lpReserved, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };

	if (realReadFileScatter == NULL)
		realReadFileScatter = (ReadFileScatterFunction)library->GetOriginalFunction(functionName);

	if (realReadFileScatter != NULL)
	{
		DWORD errorCode, returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		library->RunStandardTestsAndGetResults(logSender, ReadFileScatterCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "BOOL", &errorCode, 0, true);
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		RemoveDirectoryAReplacement
// Description: See MSDN RemoveDirectoryA function
// Parameters:	See MSDN RemoveDirectoryA parameters
// Return Value: See MSDN RemoveDirectoryA return value
//*************************************************************************
BOOL WINAPI RemoveDirectoryAReplacement(LPCSTR lpPathName)
{
	const int numParams = 1;
	char *functionName = "RemoveDirectoryA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpPathName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realRemoveDirectoryA == NULL)
		realRemoveDirectoryA = (RemoveDirectoryAFunction)library->GetOriginalFunction(functionName);
		
	if (realRemoveDirectoryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, RemoveDirectoryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		RemoveDirectoryWReplacement
// Description: See MSDN RemoveDirectoryW function
// Parameters:	See MSDN RemoveDirectoryW parameters
// Return Value: See MSDN RemoveDirectoryW return value
//*************************************************************************
BOOL WINAPI RemoveDirectoryWReplacement(LPCWSTR lpPathName)
{
	const int numParams = 1;
	char *functionName = "RemoveDirectoryW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpPathName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realRemoveDirectoryW == NULL)
		realRemoveDirectoryW = (RemoveDirectoryWFunction)library->GetOriginalFunction(functionName);
		
	if (realRemoveDirectoryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, RemoveDirectoryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		ReplaceFileAReplacement
// Description: See MSDN ReplaceFileA function
// Parameters:	See MSDN ReplaceFileA parameters
// Return Value: See MSDN ReplaceFileA return value
//*************************************************************************
BOOL WINAPI ReplaceFileAReplacement(LPCSTR lpReplacedFileName, LPCSTR lpReplacementFileName, LPCSTR lpBackupFileName, DWORD dwReplaceFlags, LPVOID lpExclude, LPVOID lpReserved)
{
	const int numParams = 6;
	char *functionName = "ReplaceFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpReplacedFileName, (char *)lpReplacementFileName, (char *)lpBackupFileName };
	void *params[numParams] = { &str[0], &str[1], &str[2], &dwReplaceFlags, &lpExclude, &lpReserved };
	ParameterType paramTypes[numParams] = { StringType, StringType, StringType, UnsignedLongType, PointerType, PointerType };
	
	if (realReplaceFileA == NULL)
		realReplaceFileA = (ReplaceFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realReplaceFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoReplacement = library->GetFileSizeInfoA (lpReplacementFileName);
		unsigned long allocInfoReplaced = library->GetFileSizeInfoA (lpReplacedFileName);
		unsigned long allocInfo = allocInfoReplacement - allocInfoReplaced;
		bool positiveAllocation = allocInfoReplacement >= allocInfoReplaced;
		
		if (library->RunStandardTestsAndGetResults(logSender, ReplaceFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ReplaceFileWReplacement
// Description: See MSDN ReplaceFileW function
// Parameters:	See MSDN ReplaceFileW parameters
// Return Value: See MSDN ReplaceFileW return value
//*************************************************************************
BOOL WINAPI ReplaceFileWReplacement(LPCWSTR lpReplacedFileName, LPCWSTR lpReplacementFileName, LPCWSTR lpBackupFileName, DWORD dwReplaceFlags, LPVOID lpExclude, LPVOID lpReserved)
{
	const int numParams = 6;
	char *functionName = "ReplaceFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpReplacedFileName, (wchar_t *)lpReplacementFileName, (wchar_t *)lpBackupFileName };
	void *params[numParams] = { &str[0], &str[1], &str[2], &dwReplaceFlags, &lpExclude, &lpReserved };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, WideStringType, UnsignedLongType, PointerType, PointerType };
	
	if (realReplaceFileW == NULL)
		realReplaceFileW = (ReplaceFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realReplaceFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		unsigned long allocInfoReplacement = library->GetFileSizeInfoW (lpReplacementFileName);
		unsigned long allocInfoReplaced = library->GetFileSizeInfoW (lpReplacedFileName);
		unsigned long allocInfo = allocInfoReplacement - allocInfoReplaced;
		bool positiveAllocation = allocInfoReplacement >= allocInfoReplaced;
		
		if (library->RunStandardTestsAndGetResults(logSender, ReplaceFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (allocInfo, positiveAllocation);
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		SearchPathAReplacement
// Description: See MSDN SearchPathA function
// Parameters:	See MSDN SearchPathA parameters
// Return Value: See MSDN SearchPathA return value
//*************************************************************************
DWORD WINAPI SearchPathAReplacement(LPCSTR lpPath, LPCSTR lpFileName, LPCSTR lpExtension, DWORD nBufferLength, LPSTR lpBuffer, LPSTR * lpFilePart)
{
	const int numParams = 6;
	char *functionName = "SearchPathA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpPath, (char *)lpFileName, (char *)lpExtension };
	void *params[numParams] = { &str[0], &str[1], &str[2], &nBufferLength, &lpBuffer, &lpFilePart };
	ParameterType paramTypes[numParams] = { StringType, StringType, StringType, UnsignedLongType, PointerType, PointerType };
	
	if (realSearchPathA == NULL)
		realSearchPathA = (SearchPathAFunction)library->GetOriginalFunction(functionName);
		
	if (realSearchPathA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		//stitch together resource path
		library->AddToPathList(1, lpFileName);
		
		if (library->RunStandardTestsAndGetResults(logSender, SearchPathACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		SearchPathWReplacement
// Description: See MSDN SearchPathW function
// Parameters:	See MSDN SearchPathW parameters
// Return Value: See MSDN SearchPathW return value
//*************************************************************************
DWORD WINAPI SearchPathWReplacement(LPCWSTR lpPath, LPCWSTR lpFileName, LPCWSTR lpExtension, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR * lpFilePart)
{
	const int numParams = 6;
	char *functionName = "SearchPathW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpPath, (wchar_t *)lpFileName, (wchar_t *)lpExtension };
	void *params[numParams] = { &str[0], &str[1], &str[2], &nBufferLength, &lpBuffer, &lpFilePart };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, WideStringType, UnsignedLongType, PointerType, PointerType };
	
	if (realSearchPathW == NULL)
		realSearchPathW = (SearchPathWFunction)library->GetOriginalFunction(functionName);
		
	if (realSearchPathW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		//stitch together resource path
		library->AddToPathList(1, lpFileName);
		
		if (library->RunStandardTestsAndGetResults(logSender, SearchPathWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		SetFileAttributesAReplacement
// Description: See MSDN SetFileAttributesA function
// Parameters:	See MSDN SetFileAttributesA parameters
// Return Value: See MSDN SetFileAttributesA return value
//*************************************************************************
BOOL WINAPI SetFileAttributesAReplacement(LPCSTR lpFileName, DWORD dwFileAttributes)
{
	const int numParams = 2;
	char *functionName = "SetFileAttributesA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &dwFileAttributes };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType };
	
	if (realSetFileAttributesA == NULL)
		realSetFileAttributesA = (SetFileAttributesAFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileAttributesA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileAttributesACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileAttributesWReplacement
// Description: See MSDN SetFileAttributesW function
// Parameters:	See MSDN SetFileAttributesW parameters
// Return Value: See MSDN SetFileAttributesW return value
//*************************************************************************
BOOL WINAPI SetFileAttributesWReplacement(LPCWSTR lpFileName, DWORD dwFileAttributes)
{
	const int numParams = 2;
	char *functionName = "SetFileAttributesW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &dwFileAttributes };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType };
	
	if (realSetFileAttributesW == NULL)
		realSetFileAttributesW = (SetFileAttributesWFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileAttributesW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileAttributesWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualAllocReplacement
// Description: See MSDN VirtualAlloc function
// Parameters:	See MSDN VirtualAlloc parameters
// Return Value: See MSDN VirtualAlloc return value
//*************************************************************************
LPVOID WINAPI VirtualAllocReplacement(LPVOID lpAddress, SIZE_T dwSize, DWORD fAllocationType, DWORD flProtect)
{
	const int numParams = 4;
	char *functionName = "VirtualAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &dwSize, &fAllocationType, &flProtect };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realVirtualAlloc == NULL)
		realVirtualAlloc = (VirtualAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, (unsigned long) dwSize, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		VirtualAllocExReplacement
// Description: See MSDN VirtualAllocEx function
// Parameters:	See MSDN VirtualAllocEx parameters
// Return Value: See MSDN VirtualAllocEx return value
//*************************************************************************
LPVOID WINAPI VirtualAllocExReplacement(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD fAllocationType, DWORD flProtect)
{
	const int numParams = 5;
	char *functionName = "VirtualAllocEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpAddress, &dwSize, &fAllocationType, &flProtect };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realVirtualAllocEx == NULL)
		realVirtualAllocEx = (VirtualAllocExFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualAllocEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualAllocExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, (unsigned long) dwSize, true))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		VirtualFreeReplacement
// Description: See MSDN VirtualFree function
// Parameters:	See MSDN VirtualFree parameters
// Return Value: See MSDN VirtualFree return value
//*************************************************************************
BOOL WINAPI VirtualFreeReplacement(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
	const int numParams = 3;
	char *functionName = "VirtualFree";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &dwSize, &dwFreeType };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realVirtualFree == NULL)
		realVirtualFree = (VirtualFreeFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualFree != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualFreeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualFreeExReplacement
// Description: See MSDN VirtualFreeEx function
// Parameters:	See MSDN VirtualFreeEx parameters
// Return Value: See MSDN VirtualFreeEx return value
//*************************************************************************
BOOL WINAPI VirtualFreeExReplacement(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
	const int numParams = 4;
	char *functionName = "VirtualFreeEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpAddress, &dwSize, &dwFreeType };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realVirtualFreeEx == NULL)
		realVirtualFreeEx = (VirtualFreeExFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualFreeEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualFreeExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualLockReplacement
// Description: See MSDN VirtualLock function
// Parameters:	See MSDN VirtualLock parameters
// Return Value: See MSDN VirtualLock return value
//*************************************************************************
BOOL WINAPI VirtualLockReplacement(LPVOID lpAddress, SIZE_T dwSize)
{
	const int numParams = 2;
	char *functionName = "VirtualLock";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &dwSize };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realVirtualLock == NULL)
		realVirtualLock = (VirtualLockFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualLock != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualLockCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualQueryReplacement
// Description: See MSDN VirtualQuery function
// Parameters:	See MSDN VirtualQuery parameters
// Return Value: See MSDN VirtualQuery return value
//*************************************************************************
SIZE_T WINAPI VirtualQueryReplacement(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength)
{
	const int numParams = 3;
	char *functionName = "VirtualQuery";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &lpBuffer, &dwLength };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType };
	
	if (realVirtualQuery == NULL)
		realVirtualQuery = (VirtualQueryFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualQuery != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualQueryCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		VirtualQueryExReplacement
// Description: See MSDN VirtualQueryEx function
// Parameters:	See MSDN VirtualQueryEx parameters
// Return Value: See MSDN VirtualQueryEx return value
//*************************************************************************
SIZE_T WINAPI VirtualQueryExReplacement(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength)
{
	const int numParams = 4;
	char *functionName = "VirtualQueryEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpAddress, &lpBuffer, &dwLength };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realVirtualQueryEx == NULL)
		realVirtualQueryEx = (VirtualQueryExFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualQueryEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SIZE_T returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualQueryExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SIZE_T", &errorCode, 0, true))
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
// Method:		VirtualUnlockReplacement
// Description: See MSDN VirtualUnlock function
// Parameters:	See MSDN VirtualUnlock parameters
// Return Value: See MSDN VirtualUnlock return value
//*************************************************************************
BOOL WINAPI VirtualUnlockReplacement(LPVOID lpAddress, SIZE_T dwSize)
{
	const int numParams = 2;
	char *functionName = "VirtualUnlock";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &dwSize };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realVirtualUnlock == NULL)
		realVirtualUnlock = (VirtualUnlockFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualUnlock != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualUnlockCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		WriteFileReplacement
// Description: See MSDN WriteFile function
// Parameters:	See MSDN WriteFile parameters
// Return Value: See MSDN WriteFile return value
//*************************************************************************
BOOL WINAPI WriteFileReplacement(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 5;
	char *functionName = "WriteFile";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberOfBytesToWrite, &lpNumberOfBytesWritten, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realWriteFile == NULL)
		realWriteFile = (WriteFileFunction)library->GetOriginalFunction(functionName);
		
	if (realWriteFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, WriteFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, nNumberOfBytesToWrite, true))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (nNumberOfBytesToWrite, true);
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
// Method:		WriteFileExReplacement
// Description: See MSDN WriteFileEx function
// Parameters:	See MSDN WriteFileEx parameters
// Return Value: See MSDN WriteFileEx return value
//*************************************************************************
BOOL WINAPI WriteFileExReplacement(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 5;
	char *functionName = "WriteFileEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberOfBytesToWrite, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realWriteFileEx == NULL)
		realWriteFileEx = (WriteFileExFunction)library->GetOriginalFunction(functionName);
		
	if (realWriteFileEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, WriteFileExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, nNumberOfBytesToWrite, true))
		{
			
		}
		else
		{
			if (tempReturnValue != 0)
				library->UpdateDiskUsage (nNumberOfBytesToWrite, true);
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
// Method:		WriteFileGatherReplacement
// Description: See MSDN WriteFileGather function
// Parameters:	See MSDN WriteFileGather parameters
// Return Value: See MSDN WriteFileGather return value
//*************************************************************************
BOOL WINAPI WriteFileGatherReplacement(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[],
    DWORD nNumberOfBytesToWrite, LPDWORD lpReserved, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 5;
	char *functionName = "WriteFileGather";
	char *categoryName = "FILE";
	void *params[numParams] = { &hFile, &aSegmentArray, &nNumberOfBytesToWrite, &lpReserved, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType };

	if (realWriteFileGather == NULL)
		realWriteFileGather = (WriteFileGatherFunction)library->GetOriginalFunction(functionName);

	if (realWriteFileGather != NULL)
	{
		DWORD errorCode, returnValue;

		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		if (!library->RunStandardTestsAndGetResults(logSender, WriteFileGatherCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue,  "BOOL", &errorCode, nNumberOfBytesToWrite, true))
		{
			if (returnValue != 0)				
				library->UpdateDiskUsage (nNumberOfBytesToWrite, true);
		}
		//clear pathlist so later tests don't get confused
		library->ClearPathList();
	
		SetLastError(errorCode);
		return returnValue;
	}

	return 0;
}
//*************************************************************************
// Method:		AllocConsoleReplacement
// Description: See MSDN AllocConsole function
// Parameters:	See MSDN AllocConsole parameters
// Return Value: See MSDN AllocConsole return value
//*************************************************************************
BOOL WINAPI AllocConsoleReplacement()
{
	const int numParams = 0;
	char *functionName = "AllocConsole";
	char *categoryName = "PROCESS";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realAllocConsole == NULL)
		realAllocConsole = (AllocConsoleFunction)library->GetOriginalFunction(functionName);
		
	if (realAllocConsole != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, AllocConsoleCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		AreFileApisANSIReplacement
// Description: See MSDN AreFileApisANSI function
// Parameters:	See MSDN AreFileApisANSI parameters
// Return Value: See MSDN AreFileApisANSI return value
//*************************************************************************
BOOL WINAPI AreFileApisANSIReplacement()
{
	const int numParams = 0;
	char *functionName = "AreFileApisANSI";
	char *categoryName = "FILE";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realAreFileApisANSI == NULL)
		realAreFileApisANSI = (AreFileApisANSIFunction)library->GetOriginalFunction(functionName);
		
	if (realAreFileApisANSI != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, AreFileApisANSICaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		AttachConsoleReplacement
// Description: See MSDN AttachConsole function
// Parameters:	See MSDN AttachConsole parameters
// Return Value: See MSDN AttachConsole return value
//*************************************************************************
BOOL WINAPI AttachConsoleReplacement(DWORD dwProcessId)
{
	const int numParams = 1;
	char *functionName = "AttachConsole";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwProcessId };
	ParameterType paramTypes[numParams] = { UnsignedLongType };
	
	if (realAttachConsole == NULL)
		realAttachConsole = (AttachConsoleFunction)library->GetOriginalFunction(functionName);
		
	if (realAttachConsole != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, AttachConsoleCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		BackupReadReplacement
// Description: See MSDN BackupRead function
// Parameters:	See MSDN BackupRead parameters
// Return Value: See MSDN BackupRead return value
//*************************************************************************
BOOL WINAPI BackupReadReplacement(HANDLE hFile, LPBYTE lpBuffer, DWORD nNumberofBytesToRead, LPDWORD lpNumberOfBytesRead, BOOL bAbort, BOOL bProcessSecurity, LPVOID * lpContext)
{
	const int numParams = 7;
	char *functionName = "BackupRead";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberofBytesToRead, &lpNumberOfBytesRead, &bAbort, &bProcessSecurity, &lpContext };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, BooleanType, BooleanType, PointerType };
	
	if (realBackupRead == NULL)
		realBackupRead = (BackupReadFunction)library->GetOriginalFunction(functionName);
		
	if (realBackupRead != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, BackupReadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		BackupSeekReplacement
// Description: See MSDN BackupSeek function
// Parameters:	See MSDN BackupSeek parameters
// Return Value: See MSDN BackupSeek return value
//*************************************************************************
BOOL WINAPI BackupSeekReplacement(HANDLE hFile, DWORD dwLowBytesToSeek, DWORD dwHighBytesToSeek, LPDWORD lpdwLowByteSeeked, LPDWORD lpdwHighByteSeeked, LPVOID * lpContext)
{
	const int numParams = 6;
	char *functionName = "BackupSeek";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &dwLowBytesToSeek, &dwHighBytesToSeek, &lpdwLowByteSeeked, &lpdwHighByteSeeked, &lpContext };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, PointerType, PointerType, PointerType };
	
	if (realBackupSeek == NULL)
		realBackupSeek = (BackupSeekFunction)library->GetOriginalFunction(functionName);
		
	if (realBackupSeek != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, BackupSeekCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		BackupWriteReplacement
// Description: See MSDN BackupWrite function
// Parameters:	See MSDN BackupWrite parameters
// Return Value: See MSDN BackupWrite return value
//*************************************************************************
BOOL WINAPI BackupWriteReplacement(HANDLE hFile, LPBYTE lpBuffer, DWORD nNumberofBytesToWrite, LPDWORD lpNumberOfBytesWritten, BOOL bAbort, BOOL bProcessSecurity, LPVOID * lpContext)
{
	const int numParams = 7;
	char *functionName = "BackupWrite";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &nNumberofBytesToWrite, &lpNumberOfBytesWritten, &bAbort, &bProcessSecurity, &lpContext };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, BooleanType, BooleanType, PointerType };
	
	if (realBackupWrite == NULL)
		realBackupWrite = (BackupWriteFunction)library->GetOriginalFunction(functionName);
		
	if (realBackupWrite != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, BackupWriteCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CancelIoReplacement
// Description: See MSDN CancelIo function
// Parameters:	See MSDN CancelIo parameters
// Return Value: See MSDN CancelIo return value
//*************************************************************************
BOOL WINAPI CancelIoReplacement(HANDLE hFile)
{
	const int numParams = 1;
	char *functionName = "CancelIo";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realCancelIo == NULL)
		realCancelIo = (CancelIoFunction)library->GetOriginalFunction(functionName);
		
	if (realCancelIo != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, CancelIoCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CloseHandleReplacement
// Description: See MSDN CloseHandle function
// Parameters:	See MSDN CloseHandle parameters
// Return Value: See MSDN CloseHandle return value
//*************************************************************************
BOOL WINAPI CloseHandleReplacement(HANDLE hObject)
{
	const int numParams = 1;
	char *functionName = "CloseHandle";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hObject };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realCloseHandle == NULL)
		realCloseHandle = (CloseHandleFunction)library->GetOriginalFunction(functionName);
		
	if (realCloseHandle != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CloseHandleCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateFiberReplacement
// Description: See MSDN CreateFiber function
// Parameters:	See MSDN CreateFiber parameters
// Return Value: See MSDN CreateFiber return value
//*************************************************************************
LPVOID WINAPI CreateFiberReplacement(SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
	const int numParams = 3;
	char *functionName = "CreateFiber";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &dwStackSize, &lpStartAddress, &lpParameter };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType, PointerType };
	
	if (realCreateFiber == NULL)
		realCreateFiber = (CreateFiberFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFiber != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFiberCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, 0, true))
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
// Method:		CreateFiberExReplacement
// Description: See MSDN CreateFiberEx function
// Parameters:	See MSDN CreateFiberEx parameters
// Return Value: See MSDN CreateFiberEx return value
//*************************************************************************
LPVOID WINAPI CreateFiberExReplacement(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
	const int numParams = 5;
	char *functionName = "CreateFiberEx";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &dwStackCommitSize, &dwStackReserveSize, &dwFlags, &lpStartAddress, &lpParameter };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, UnsignedLongType, PointerType, PointerType };
	
	if (realCreateFiberEx == NULL)
		realCreateFiberEx = (CreateFiberExFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFiberEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPVOID returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFiberExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPVOID", &errorCode, 0, true))
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
// Method:		CreateFileMappingAReplacement
// Description: See MSDN CreateFileMappingA function
// Parameters:	See MSDN CreateFileMappingA parameters
// Return Value: See MSDN CreateFileMappingA return value
//*************************************************************************
HANDLE WINAPI CreateFileMappingAReplacement(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName)
{
	const int numParams = 6;
	char *functionName = "CreateFileMappingA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpName };
	void *params[numParams] = { &hFile, &lpFileMappingAttributes, &flProtect, &dwMaximumSizeHigh, &dwMaximumSizeLow, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, StringType };
	
	if (realCreateFileMappingA == NULL)
		realCreateFileMappingA = (CreateFileMappingAFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFileMappingA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFileMappingACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		CreateFileMappingWReplacement
// Description: See MSDN CreateFileMappingW function
// Parameters:	See MSDN CreateFileMappingW parameters
// Return Value: See MSDN CreateFileMappingW return value
//*************************************************************************
HANDLE WINAPI CreateFileMappingWReplacement(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName)
{
	const int numParams = 6;
	char *functionName = "CreateFileMappingW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpName };
	void *params[numParams] = { &hFile, &lpFileMappingAttributes, &flProtect, &dwMaximumSizeHigh, &dwMaximumSizeLow, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, WideStringType };
	
	if (realCreateFileMappingW == NULL)
		realCreateFileMappingW = (CreateFileMappingWFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateFileMappingW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateFileMappingWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		CreateHardLinkAReplacement
// Description: See MSDN CreateHardLinkA function
// Parameters:	See MSDN CreateHardLinkA parameters
// Return Value: See MSDN CreateHardLinkA return value
//*************************************************************************
BOOL WINAPI CreateHardLinkAReplacement(LPCSTR lpFileName, LPCSTR lpExistingFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "CreateHardLinkA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName, (char *)lpExistingFileName };
	void *params[numParams] = { &str[0], &str[1], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType };
	
	if (realCreateHardLinkA == NULL)
		realCreateHardLinkA = (CreateHardLinkAFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateHardLinkA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateHardLinkACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateHardLinkWReplacement
// Description: See MSDN CreateHardLinkW function
// Parameters:	See MSDN CreateHardLinkW parameters
// Return Value: See MSDN CreateHardLinkW return value
//*************************************************************************
BOOL WINAPI CreateHardLinkWReplacement(LPCWSTR lpFileName, LPCWSTR lpExistingFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	const int numParams = 3;
	char *functionName = "CreateHardLinkW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName, (wchar_t *)lpExistingFileName };
	void *params[numParams] = { &str[0], &str[1], &lpSecurityAttributes };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType };
	
	if (realCreateHardLinkW == NULL)
		realCreateHardLinkW = (CreateHardLinkWFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateHardLinkW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateHardLinkWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateMemoryResourceNotificationReplacement
// Description: See MSDN CreateMemoryResourceNotification function
// Parameters:	See MSDN CreateMemoryResourceNotification parameters
// Return Value: See MSDN CreateMemoryResourceNotification return value
//*************************************************************************
HANDLE WINAPI CreateMemoryResourceNotificationReplacement(MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType)
{
	const int numParams = 1;
	char *functionName = "CreateMemoryResourceNotification";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &NotificationType };
	ParameterType paramTypes[numParams] = { IntegerType };
	
	if (realCreateMemoryResourceNotification == NULL)
		realCreateMemoryResourceNotification = (CreateMemoryResourceNotificationFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateMemoryResourceNotification != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateMemoryResourceNotificationCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		CreateThreadReplacement
// Description: See MSDN CreateThread function
// Parameters:	See MSDN CreateThread parameters
// Return Value: See MSDN CreateThread return value
//*************************************************************************
HANDLE WINAPI CreateThreadReplacement(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	const int numParams = 6;
	char *functionName = "CreateThread";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpThreadAttributes, &dwStackSize, &lpStartAddress, &lpParameter, &dwCreationFlags, &lpThreadId };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, PointerType, UnsignedLongType, PointerType };
	
	if (realCreateThread == NULL)
		realCreateThread = (CreateThreadFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateThread != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateThreadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		CreateToolhelp32SnapshotReplacement
// Description: See MSDN CreateToolhelp32Snapshot function
// Parameters:	See MSDN CreateToolhelp32Snapshot parameters
// Return Value: See MSDN CreateToolhelp32Snapshot return value
//*************************************************************************
HANDLE WINAPI CreateToolhelp32SnapshotReplacement(DWORD dwFlags, DWORD th32ProcessID)
{
	const int numParams = 2;
	char *functionName = "CreateToolhelp32Snapshot";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwFlags, &th32ProcessID };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType };
	
	if (realCreateToolhelp32Snapshot == NULL)
		realCreateToolhelp32Snapshot = (CreateToolhelp32SnapshotFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateToolhelp32Snapshot != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateToolhelp32SnapshotCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		ContinueDebugEventReplacement
// Description: See MSDN ContinueDebugEvent function
// Parameters:	See MSDN ContinueDebugEvent parameters
// Return Value: See MSDN ContinueDebugEvent return value
//*************************************************************************
BOOL WINAPI ContinueDebugEventReplacement(DWORD dwProcessId, DWORD dwThreadId, DWORD dwContinueStatus)
{
	const int numParams = 3;
	char *functionName = "ContinueDebugEvent";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwProcessId, &dwThreadId, &dwContinueStatus };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realContinueDebugEvent == NULL)
		realContinueDebugEvent = (ContinueDebugEventFunction)library->GetOriginalFunction(functionName);
		
	if (realContinueDebugEvent != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ContinueDebugEventCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DebugActiveProcessReplacement
// Description: See MSDN DebugActiveProcess function
// Parameters:	See MSDN DebugActiveProcess parameters
// Return Value: See MSDN DebugActiveProcess return value
//*************************************************************************
BOOL WINAPI DebugActiveProcessReplacement(DWORD dwProcessId)
{
	const int numParams = 1;
	char *functionName = "DebugActiveProcess";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwProcessId };
	ParameterType paramTypes[numParams] = { UnsignedLongType };
	
	if (realDebugActiveProcess == NULL)
		realDebugActiveProcess = (DebugActiveProcessFunction)library->GetOriginalFunction(functionName);
		
	if (realDebugActiveProcess != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DebugActiveProcessCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DebugActiveProcessStopReplacement
// Description: See MSDN DebugActiveProcessStop function
// Parameters:	See MSDN DebugActiveProcessStop parameters
// Return Value: See MSDN DebugActiveProcessStop return value
//*************************************************************************
BOOL WINAPI DebugActiveProcessStopReplacement(DWORD dwProcessId)
{
	const int numParams = 1;
	char *functionName = "DebugActiveProcessStop";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwProcessId };
	ParameterType paramTypes[numParams] = { UnsignedLongType };
	
	if (realDebugActiveProcessStop == NULL)
		realDebugActiveProcessStop = (DebugActiveProcessStopFunction)library->GetOriginalFunction(functionName);
		
	if (realDebugActiveProcessStop != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DebugActiveProcessStopCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DebugBreakReplacement
// Description: See MSDN DebugBreak function
// Parameters:	See MSDN DebugBreak parameters
// Return Value: See MSDN DebugBreak return value
//*************************************************************************
VOID WINAPI DebugBreakReplacement()
{
	const int numParams = 0;
	char *functionName = "DebugBreak";
	char *categoryName = "PROCESS";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realDebugBreak == NULL)
		realDebugBreak = (DebugBreakFunction)library->GetOriginalFunction(functionName);
		
	if (realDebugBreak != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DebugBreakCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		DebugBreakProcessReplacement
// Description: See MSDN DebugBreakProcess function
// Parameters:	See MSDN DebugBreakProcess parameters
// Return Value: See MSDN DebugBreakProcess return value
//*************************************************************************
BOOL WINAPI DebugBreakProcessReplacement(HANDLE Process)
{
	const int numParams = 1;
	char *functionName = "DebugBreakProcess";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &Process };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realDebugBreakProcess == NULL)
		realDebugBreakProcess = (DebugBreakProcessFunction)library->GetOriginalFunction(functionName);
		
	if (realDebugBreakProcess != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DebugBreakProcessCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DebugSetProcessKillOnExitReplacement
// Description: See MSDN DebugSetProcessKillOnExit function
// Parameters:	See MSDN DebugSetProcessKillOnExit parameters
// Return Value: See MSDN DebugSetProcessKillOnExit return value
//*************************************************************************
BOOL WINAPI DebugSetProcessKillOnExitReplacement(BOOL KillOnExit)
{
	const int numParams = 1;
	char *functionName = "DebugSetProcessKillOnExit";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &KillOnExit };
	ParameterType paramTypes[numParams] = { BooleanType };
	
	if (realDebugSetProcessKillOnExit == NULL)
		realDebugSetProcessKillOnExit = (DebugSetProcessKillOnExitFunction)library->GetOriginalFunction(functionName);
		
	if (realDebugSetProcessKillOnExit != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DebugSetProcessKillOnExitCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FatalAppExitAReplacement
// Description: See MSDN FatalAppExitA function
// Parameters:	See MSDN FatalAppExitA parameters
// Return Value: See MSDN FatalAppExitA return value
//*************************************************************************
VOID WINAPI FatalAppExitAReplacement(UINT uAction, LPCSTR lpMessageText)
{
	const int numParams = 2;
	char *functionName = "FatalAppExitA";
	char *categoryName = "PROCESS";
	SiString str[] = { (char *)lpMessageText };
	void *params[numParams] = { &uAction, &str[0] };
	ParameterType paramTypes[numParams] = { IntegerType, StringType };
	
	if (realFatalAppExitA == NULL)
		realFatalAppExitA = (FatalAppExitAFunction)library->GetOriginalFunction(functionName);
		
	if (realFatalAppExitA != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FatalAppExitACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		FatalAppExitWReplacement
// Description: See MSDN FatalAppExitW function
// Parameters:	See MSDN FatalAppExitW parameters
// Return Value: See MSDN FatalAppExitW return value
//*************************************************************************
VOID WINAPI FatalAppExitWReplacement(UINT uAction, LPCWSTR lpMessageText)
{
	const int numParams = 2;
	char *functionName = "FatalAppExitW";
	char *categoryName = "PROCESS";
	SiString str[] = { (wchar_t *)lpMessageText };
	void *params[numParams] = { &uAction, &str[0] };
	ParameterType paramTypes[numParams] = { IntegerType, WideStringType };
	
	if (realFatalAppExitW == NULL)
		realFatalAppExitW = (FatalAppExitWFunction)library->GetOriginalFunction(functionName);
		
	if (realFatalAppExitW != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FatalAppExitWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		FatalExitReplacement
// Description: See MSDN FatalExit function
// Parameters:	See MSDN FatalExit parameters
// Return Value: See MSDN FatalExit return value
//*************************************************************************
VOID WINAPI FatalExitReplacement(int ExitCode)
{
	const int numParams = 1;
	char *functionName = "FatalExit";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &ExitCode };
	ParameterType paramTypes[numParams] = { IntegerType };
	
	if (realFatalExit == NULL)
		realFatalExit = (FatalExitFunction)library->GetOriginalFunction(functionName);
		
	if (realFatalExit != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FatalExitCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		DeleteFiberReplacement
// Description: See MSDN DeleteFiber function
// Parameters:	See MSDN DeleteFiber parameters
// Return Value: See MSDN DeleteFiber return value
//*************************************************************************
VOID WINAPI DeleteFiberReplacement(LPVOID lpFiber)
{
	const int numParams = 1;
	char *functionName = "DeleteFiber";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &lpFiber };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realDeleteFiber == NULL)
		realDeleteFiber = (DeleteFiberFunction)library->GetOriginalFunction(functionName);
		
	if (realDeleteFiber != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DeleteFiberCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		DisableThreadLibraryCallsReplacement
// Description: See MSDN DisableThreadLibraryCalls function
// Parameters:	See MSDN DisableThreadLibraryCalls parameters
// Return Value: See MSDN DisableThreadLibraryCalls return value
//*************************************************************************
BOOL WINAPI DisableThreadLibraryCallsReplacement(HMODULE hLibModule)
{
	const int numParams = 1;
	char *functionName = "DisableThreadLibraryCalls";
	char *categoryName = "LIBRARY";
	
	void *params[numParams] = { &hLibModule };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realDisableThreadLibraryCalls == NULL)
		realDisableThreadLibraryCalls = (DisableThreadLibraryCallsFunction)library->GetOriginalFunction(functionName);
		
	if (realDisableThreadLibraryCalls != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DisableThreadLibraryCallsCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DnsHostnameToComputerNameAReplacement
// Description: See MSDN DnsHostnameToComputerNameA function
// Parameters:	See MSDN DnsHostnameToComputerNameA parameters
// Return Value: See MSDN DnsHostnameToComputerNameA return value
//*************************************************************************
BOOL WINAPI DnsHostnameToComputerNameAReplacement(LPCSTR Hostname, LPSTR ComputerName, LPDWORD nSize)
{
	const int numParams = 3;
	char *functionName = "DnsHostnameToComputerNameA";
	char *categoryName = "NETWORK";
	SiString str[] = { (char *)Hostname, (char *)ComputerName };
	void *params[numParams] = { &str[0], &str[1], &nSize };
	ParameterType paramTypes[numParams] = { StringType, StringType, PointerType };
	
	if (realDnsHostnameToComputerNameA == NULL)
		realDnsHostnameToComputerNameA = (DnsHostnameToComputerNameAFunction)library->GetOriginalFunction(functionName);
		
	if (realDnsHostnameToComputerNameA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DnsHostnameToComputerNameACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		DnsHostnameToComputerNameWReplacement
// Description: See MSDN DnsHostnameToComputerNameW function
// Parameters:	See MSDN DnsHostnameToComputerNameW parameters
// Return Value: See MSDN DnsHostnameToComputerNameW return value
//*************************************************************************
BOOL WINAPI DnsHostnameToComputerNameWReplacement(LPCWSTR Hostname, LPWSTR ComputerName, LPDWORD nSize)
{
	const int numParams = 3;
	char *functionName = "DnsHostnameToComputerNameW";
	char *categoryName = "NETWORK";
	SiString str[] = { (wchar_t *)Hostname, (wchar_t *)ComputerName };
	void *params[numParams] = { &str[0], &str[1], &nSize };
	ParameterType paramTypes[numParams] = { WideStringType, WideStringType, PointerType };
	
	if (realDnsHostnameToComputerNameW == NULL)
		realDnsHostnameToComputerNameW = (DnsHostnameToComputerNameWFunction)library->GetOriginalFunction(functionName);
		
	if (realDnsHostnameToComputerNameW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DnsHostnameToComputerNameWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FindFirstFileAReplacement
// Description: See MSDN FindFirstFileA function
// Parameters:	See MSDN FindFirstFileA parameters
// Return Value: See MSDN FindFirstFileA return value
//*************************************************************************
HANDLE WINAPI FindFirstFileAReplacement(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	const int numParams = 2;
	char *functionName = "FindFirstFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &lpFindFileData };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realFindFirstFileA == NULL)
		realFindFirstFileA = (FindFirstFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realFindFirstFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindFirstFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		FindFirstFileWReplacement
// Description: See MSDN FindFirstFileW function
// Parameters:	See MSDN FindFirstFileW parameters
// Return Value: See MSDN FindFirstFileW return value
//*************************************************************************
HANDLE WINAPI FindFirstFileWReplacement(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
	const int numParams = 2;
	char *functionName = "FindFirstFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &lpFindFileData };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realFindFirstFileW == NULL)
		realFindFirstFileW = (FindFirstFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realFindFirstFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindFirstFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		FindFirstFileExAReplacement
// Description: See MSDN FindFirstFileExA function
// Parameters:	See MSDN FindFirstFileExA parameters
// Return Value: See MSDN FindFirstFileExA return value
//*************************************************************************
HANDLE WINAPI FindFirstFileExAReplacement(LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
{
	const int numParams = 6;
	char *functionName = "FindFirstFileExA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &fInfoLevelId, &lpFindFileData, &fSearchOp, &lpSearchFilter, &dwAdditionalFlags };
	ParameterType paramTypes[numParams] = { StringType, IntegerType, PointerType, IntegerType, PointerType, UnsignedLongType };
	
	if (realFindFirstFileExA == NULL)
		realFindFirstFileExA = (FindFirstFileExAFunction)library->GetOriginalFunction(functionName);
		
	if (realFindFirstFileExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindFirstFileExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		FindFirstFileExWReplacement
// Description: See MSDN FindFirstFileExW function
// Parameters:	See MSDN FindFirstFileExW parameters
// Return Value: See MSDN FindFirstFileExW return value
//*************************************************************************
HANDLE WINAPI FindFirstFileExWReplacement(LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
{
	const int numParams = 6;
	char *functionName = "FindFirstFileExW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &fInfoLevelId, &lpFindFileData, &fSearchOp, &lpSearchFilter, &dwAdditionalFlags };
	ParameterType paramTypes[numParams] = { WideStringType, IntegerType, PointerType, IntegerType, PointerType, UnsignedLongType };
	
	if (realFindFirstFileExW == NULL)
		realFindFirstFileExW = (FindFirstFileExWFunction)library->GetOriginalFunction(functionName);
		
	if (realFindFirstFileExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindFirstFileExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		FindNextFileAReplacement
// Description: See MSDN FindNextFileA function
// Parameters:	See MSDN FindNextFileA parameters
// Return Value: See MSDN FindNextFileA return value
//*************************************************************************
BOOL WINAPI FindNextFileAReplacement(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	const int numParams = 2;
	char *functionName = "FindNextFileA";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFindFile, &lpFindFileData };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realFindNextFileA == NULL)
		realFindNextFileA = (FindNextFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realFindNextFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindNextFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FindNextFileWReplacement
// Description: See MSDN FindNextFileW function
// Parameters:	See MSDN FindNextFileW parameters
// Return Value: See MSDN FindNextFileW return value
//*************************************************************************
BOOL WINAPI FindNextFileWReplacement(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	const int numParams = 2;
	char *functionName = "FindNextFileW";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFindFile, &lpFindFileData };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realFindNextFileW == NULL)
		realFindNextFileW = (FindNextFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realFindNextFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FindNextFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetCompressedFileSizeAReplacement
// Description: See MSDN GetCompressedFileSizeA function
// Parameters:	See MSDN GetCompressedFileSizeA parameters
// Return Value: See MSDN GetCompressedFileSizeA return value
//*************************************************************************
DWORD WINAPI GetCompressedFileSizeAReplacement(LPCSTR lpFileName, LPDWORD lpFileSizeHigh)
{
	const int numParams = 2;
	char *functionName = "GetCompressedFileSizeA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &lpFileSizeHigh };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realGetCompressedFileSizeA == NULL)
		realGetCompressedFileSizeA = (GetCompressedFileSizeAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetCompressedFileSizeA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetCompressedFileSizeACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetCompressedFileSizeWReplacement
// Description: See MSDN GetCompressedFileSizeW function
// Parameters:	See MSDN GetCompressedFileSizeW parameters
// Return Value: See MSDN GetCompressedFileSizeW return value
//*************************************************************************
DWORD WINAPI GetCompressedFileSizeWReplacement(LPCWSTR lpFileName, LPDWORD lpFileSizeHigh)
{
	const int numParams = 2;
	char *functionName = "GetCompressedFileSizeW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &lpFileSizeHigh };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType };
	
	if (realGetCompressedFileSizeW == NULL)
		realGetCompressedFileSizeW = (GetCompressedFileSizeWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetCompressedFileSizeW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetCompressedFileSizeWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetExitCodeProcessReplacement
// Description: See MSDN GetExitCodeProcess function
// Parameters:	See MSDN GetExitCodeProcess parameters
// Return Value: See MSDN GetExitCodeProcess return value
//*************************************************************************
BOOL WINAPI GetExitCodeProcessReplacement(HANDLE hProcess, LPDWORD lpExitCode)
{
	const int numParams = 2;
	char *functionName = "GetExitCodeProcess";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hProcess, &lpExitCode };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetExitCodeProcess == NULL)
		realGetExitCodeProcess = (GetExitCodeProcessFunction)library->GetOriginalFunction(functionName);
		
	if (realGetExitCodeProcess != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetExitCodeProcessCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetExitCodeThreadReplacement
// Description: See MSDN GetExitCodeThread function
// Parameters:	See MSDN GetExitCodeThread parameters
// Return Value: See MSDN GetExitCodeThread return value
//*************************************************************************
BOOL WINAPI GetExitCodeThreadReplacement(HANDLE hThread, LPDWORD lpExitCode)
{
	const int numParams = 2;
	char *functionName = "GetExitCodeThread";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hThread, &lpExitCode };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetExitCodeThread == NULL)
		realGetExitCodeThread = (GetExitCodeThreadFunction)library->GetOriginalFunction(functionName);
		
	if (realGetExitCodeThread != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetExitCodeThreadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFullPathNameAReplacement
// Description: See MSDN GetFullPathNameA function
// Parameters:	See MSDN GetFullPathNameA parameters
// Return Value: See MSDN GetFullPathNameA return value
//*************************************************************************
DWORD WINAPI GetFullPathNameAReplacement(LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR * lpFilePart)
{
	const int numParams = 4;
	char *functionName = "GetFullPathNameA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &nBufferLength, &lpBuffer, &lpFilePart };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType, PointerType, PointerType };
	
	if (realGetFullPathNameA == NULL)
		realGetFullPathNameA = (GetFullPathNameAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFullPathNameA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFullPathNameACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetFullPathNameWReplacement
// Description: See MSDN GetFullPathNameW function
// Parameters:	See MSDN GetFullPathNameW parameters
// Return Value: See MSDN GetFullPathNameW return value
//*************************************************************************
DWORD WINAPI GetFullPathNameWReplacement(LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR * lpFilePart)
{
	const int numParams = 4;
	char *functionName = "GetFullPathNameW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &nBufferLength, &lpBuffer, &lpFilePart };
	ParameterType paramTypes[numParams] = { WideStringType, UnsignedLongType, PointerType, PointerType };
	
	if (realGetFullPathNameW == NULL)
		realGetFullPathNameW = (GetFullPathNameWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFullPathNameW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFullPathNameWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetProcessHeapReplacement
// Description: See MSDN GetProcessHeap function
// Parameters:	See MSDN GetProcessHeap parameters
// Return Value: See MSDN GetProcessHeap return value
//*************************************************************************
HANDLE WINAPI GetProcessHeapReplacement()
{
	const int numParams = 0;
	char *functionName = "GetProcessHeap";
	char *categoryName = "MEMORY";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realGetProcessHeap == NULL)
		realGetProcessHeap = (GetProcessHeapFunction)library->GetOriginalFunction(functionName);
		
	if (realGetProcessHeap != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetProcessHeapCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		GetProcessHeapsReplacement
// Description: See MSDN GetProcessHeaps function
// Parameters:	See MSDN GetProcessHeaps parameters
// Return Value: See MSDN GetProcessHeaps return value
//*************************************************************************
DWORD WINAPI GetProcessHeapsReplacement(DWORD NumberOfHeaps, PHANDLE ProcessHeaps)
{
	const int numParams = 2;
	char *functionName = "GetProcessHeaps";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &NumberOfHeaps, &ProcessHeaps };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType };
	
	if (realGetProcessHeaps == NULL)
		realGetProcessHeaps = (GetProcessHeapsFunction)library->GetOriginalFunction(functionName);
		
	if (realGetProcessHeaps != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetProcessHeapsCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		GetProcessIoCountersReplacement
// Description: See MSDN GetProcessIoCounters function
// Parameters:	See MSDN GetProcessIoCounters parameters
// Return Value: See MSDN GetProcessIoCounters return value
//*************************************************************************
BOOL WINAPI GetProcessIoCountersReplacement(HANDLE hProcess, PIO_COUNTERS lpIoCounters)
{
	const int numParams = 2;
	char *functionName = "GetProcessIoCounters";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hProcess, &lpIoCounters };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realGetProcessIoCounters == NULL)
		realGetProcessIoCounters = (GetProcessIoCountersFunction)library->GetOriginalFunction(functionName);
		
	if (realGetProcessIoCounters != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetProcessIoCountersCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetProcessTimesReplacement
// Description: See MSDN GetProcessTimes function
// Parameters:	See MSDN GetProcessTimes parameters
// Return Value: See MSDN GetProcessTimes return value
//*************************************************************************
BOOL WINAPI GetProcessTimesReplacement(HANDLE hProcess, LPFILETIME lpCreationTime, LPFILETIME lpExitTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime)
{
	const int numParams = 5;
	char *functionName = "GetProcessTimes";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hProcess, &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, PointerType, PointerType };
	
	if (realGetProcessTimes == NULL)
		realGetProcessTimes = (GetProcessTimesFunction)library->GetOriginalFunction(functionName);
		
	if (realGetProcessTimes != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetProcessTimesCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetProcessWorkingSetSizeReplacement
// Description: See MSDN GetProcessWorkingSetSize function
// Parameters:	See MSDN GetProcessWorkingSetSize parameters
// Return Value: See MSDN GetProcessWorkingSetSize return value
//*************************************************************************
BOOL WINAPI GetProcessWorkingSetSizeReplacement(HANDLE hProcess, PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize)
{
	const int numParams = 3;
	char *functionName = "GetProcessWorkingSetSize";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpMinimumWorkingSetSize, &lpMaximumWorkingSetSize };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realGetProcessWorkingSetSize == NULL)
		realGetProcessWorkingSetSize = (GetProcessWorkingSetSizeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetProcessWorkingSetSize != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetProcessWorkingSetSizeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetStartupInfoAReplacement
// Description: See MSDN GetStartupInfoA function
// Parameters:	See MSDN GetStartupInfoA parameters
// Return Value: See MSDN GetStartupInfoA return value
//*************************************************************************
VOID WINAPI GetStartupInfoAReplacement(LPSTARTUPINFOA lpStartupInfo)
{
	const int numParams = 1;
	char *functionName = "GetStartupInfoA";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpStartupInfo };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetStartupInfoA == NULL)
		realGetStartupInfoA = (GetStartupInfoAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetStartupInfoA != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetStartupInfoACaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetStartupInfoWReplacement
// Description: See MSDN GetStartupInfoW function
// Parameters:	See MSDN GetStartupInfoW parameters
// Return Value: See MSDN GetStartupInfoW return value
//*************************************************************************
VOID WINAPI GetStartupInfoWReplacement(LPSTARTUPINFOW lpStartupInfo)
{
	const int numParams = 1;
	char *functionName = "GetStartupInfoW";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpStartupInfo };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetStartupInfoW == NULL)
		realGetStartupInfoW = (GetStartupInfoWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetStartupInfoW != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetStartupInfoWCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetSystemDirectoryAReplacement
// Description: See MSDN GetSystemDirectoryA function
// Parameters:	See MSDN GetSystemDirectoryA parameters
// Return Value: See MSDN GetSystemDirectoryA return value
//*************************************************************************
UINT WINAPI GetSystemDirectoryAReplacement(LPSTR lpBuffer, UINT uSize)
{
	const int numParams = 2;
	char *functionName = "GetSystemDirectoryA";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &lpBuffer, &uSize };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realGetSystemDirectoryA == NULL)
		realGetSystemDirectoryA = (GetSystemDirectoryAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemDirectoryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemDirectoryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetSystemDirectoryWReplacement
// Description: See MSDN GetSystemDirectoryW function
// Parameters:	See MSDN GetSystemDirectoryW parameters
// Return Value: See MSDN GetSystemDirectoryW return value
//*************************************************************************
UINT WINAPI GetSystemDirectoryWReplacement(LPWSTR lpBuffer, UINT uSize)
{
	const int numParams = 2;
	char *functionName = "GetSystemDirectoryW";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &lpBuffer, &uSize };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realGetSystemDirectoryW == NULL)
		realGetSystemDirectoryW = (GetSystemDirectoryWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemDirectoryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemDirectoryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetSystemWindowsDirectoryAReplacement
// Description: See MSDN GetSystemWindowsDirectoryA function
// Parameters:	See MSDN GetSystemWindowsDirectoryA parameters
// Return Value: See MSDN GetSystemWindowsDirectoryA return value
//*************************************************************************
UINT WINAPI GetSystemWindowsDirectoryAReplacement(LPSTR lpBuffer, UINT uSize)
{
	const int numParams = 2;
	char *functionName = "GetSystemWindowsDirectoryA";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &lpBuffer, &uSize };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realGetSystemWindowsDirectoryA == NULL)
		realGetSystemWindowsDirectoryA = (GetSystemWindowsDirectoryAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemWindowsDirectoryA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemWindowsDirectoryACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetSystemWindowsDirectoryWReplacement
// Description: See MSDN GetSystemWindowsDirectoryW function
// Parameters:	See MSDN GetSystemWindowsDirectoryW parameters
// Return Value: See MSDN GetSystemWindowsDirectoryW return value
//*************************************************************************
UINT WINAPI GetSystemWindowsDirectoryWReplacement(LPWSTR lpBuffer, UINT uSize)
{
	const int numParams = 2;
	char *functionName = "GetSystemWindowsDirectoryW";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &lpBuffer, &uSize };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realGetSystemWindowsDirectoryW == NULL)
		realGetSystemWindowsDirectoryW = (GetSystemWindowsDirectoryWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemWindowsDirectoryW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemWindowsDirectoryWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GetWriteWatchReplacement
// Description: See MSDN GetWriteWatch function
// Parameters:	See MSDN GetWriteWatch parameters
// Return Value: See MSDN GetWriteWatch return value
//*************************************************************************
UINT WINAPI GetWriteWatchReplacement(DWORD dwFlags, PVOID lpBaseAddress, SIZE_T dwRegionSize, PVOID * lpAddresses, PULONG_PTR lpdwCount, PULONG lpdwGranularity)
{
	const int numParams = 6;
	char *functionName = "GetWriteWatch";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &dwFlags, &lpBaseAddress, &dwRegionSize, &lpAddresses, &lpdwCount, &lpdwGranularity };
	ParameterType paramTypes[numParams] = { UnsignedLongType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType };
	
	if (realGetWriteWatch == NULL)
		realGetWriteWatch = (GetWriteWatchFunction)library->GetOriginalFunction(functionName);
		
	if (realGetWriteWatch != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetWriteWatchCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		GlobalMemoryStatusReplacement
// Description: See MSDN GlobalMemoryStatus function
// Parameters:	See MSDN GlobalMemoryStatus parameters
// Return Value: See MSDN GlobalMemoryStatus return value
//*************************************************************************
VOID WINAPI GlobalMemoryStatusReplacement(LPMEMORYSTATUS lpBuffer)
{
	const int numParams = 1;
	char *functionName = "GlobalMemoryStatus";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpBuffer };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGlobalMemoryStatus == NULL)
		realGlobalMemoryStatus = (GlobalMemoryStatusFunction)library->GetOriginalFunction(functionName);
		
	if (realGlobalMemoryStatus != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GlobalMemoryStatusCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GlobalReAllocReplacement
// Description: See MSDN GlobalReAlloc function
// Parameters:	See MSDN GlobalReAlloc parameters
// Return Value: See MSDN GlobalReAlloc return value
//*************************************************************************
HGLOBAL WINAPI GlobalReAllocReplacement(HGLOBAL hMem, SIZE_T dwBytes, UINT uFlags)
{
	const int numParams = 3;
	char *functionName = "GlobalReAlloc";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hMem, &dwBytes, &uFlags };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, IntegerType };
	
	if (realGlobalReAlloc == NULL)
		realGlobalReAlloc = (GlobalReAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realGlobalReAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HGLOBAL returnValue;
		SIZE_T allocInfo = GlobalSize(hMem);
		bool positiveAllocation = dwBytes >= allocInfo;

		if (positiveAllocation)
			allocInfo = dwBytes - allocInfo;
		else
			allocInfo = allocInfo - dwBytes;
		
		if (library->RunStandardTestsAndGetResults(logSender, GlobalReAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HGLOBAL", &errorCode, allocInfo, positiveAllocation))
		{
			
		}
		else
		{
			library->UpdateMemoryUsage (library->GetProcessMemory());
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		LockFileReplacement
// Description: See MSDN LockFile function
// Parameters:	See MSDN LockFile parameters
// Return Value: See MSDN LockFile return value
//*************************************************************************
BOOL WINAPI LockFileReplacement(HANDLE hFile, DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh, DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh)
{
	const int numParams = 5;
	char *functionName = "LockFile";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &dwFileOffsetLow, &dwFileOffsetHigh, &nNumberOfBytesToLockLow, &nNumberOfBytesToLockHigh };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realLockFile == NULL)
		realLockFile = (LockFileFunction)library->GetOriginalFunction(functionName);
		
	if (realLockFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, LockFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		LockFileExReplacement
// Description: See MSDN LockFileEx function
// Parameters:	See MSDN LockFileEx parameters
// Return Value: See MSDN LockFileEx return value
//*************************************************************************
BOOL WINAPI LockFileExReplacement(HANDLE hFile, DWORD dwFlags, DWORD dwReserved, DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 6;
	char *functionName = "LockFileEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &dwFlags, &dwReserved, &nNumberOfBytesToLockLow, &nNumberOfBytesToLockHigh, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realLockFileEx == NULL)
		realLockFileEx = (LockFileExFunction)library->GetOriginalFunction(functionName);
		
	if (realLockFileEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, LockFileExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		LZCloseReplacement
// Description: See MSDN LZClose function
// Parameters:	See MSDN LZClose parameters
// Return Value: See MSDN LZClose return value
//*************************************************************************
VOID APIENTRY LZCloseReplacement(INT hFile)
{
	const int numParams = 1;
	char *functionName = "LZClose";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile };
	ParameterType paramTypes[numParams] = { IntegerType };
	
	if (realLZClose == NULL)
		realLZClose = (LZCloseFunction)library->GetOriginalFunction(functionName);
		
	if (realLZClose != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZCloseCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		LZCopyReplacement
// Description: See MSDN LZCopy function
// Parameters:	See MSDN LZCopy parameters
// Return Value: See MSDN LZCopy return value
//*************************************************************************
VOID APIENTRY LZCopyReplacement(INT hfSource, INT hfDest)
{
	const int numParams = 2;
	char *functionName = "LZCopy";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hfSource, &hfDest };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType };
	
	if (realLZCopy == NULL)
		realLZCopy = (LZCopyFunction)library->GetOriginalFunction(functionName);
		
	if (realLZCopy != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZCopyCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		LZOpenFileAReplacement
// Description: See MSDN LZOpenFileA function
// Parameters:	See MSDN LZOpenFileA parameters
// Return Value: See MSDN LZOpenFileA return value
//*************************************************************************
INT APIENTRY LZOpenFileAReplacement(LPSTR lpFileName, LPOFSTRUCT lpReOpenBuf, WORD wStyle)
{
	const int numParams = 3;
	char *functionName = "LZOpenFileA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &lpReOpenBuf, &wStyle };
	ParameterType paramTypes[numParams] = { StringType, PointerType, IntegerType };
	
	if (realLZOpenFileA == NULL)
		realLZOpenFileA = (LZOpenFileAFunction)library->GetOriginalFunction(functionName);
		
	if (realLZOpenFileA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		INT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZOpenFileACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "INT", &errorCode, 0, true))
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
// Method:		LZOpenFileWReplacement
// Description: See MSDN LZOpenFileW function
// Parameters:	See MSDN LZOpenFileW parameters
// Return Value: See MSDN LZOpenFileW return value
//*************************************************************************
INT APIENTRY LZOpenFileWReplacement(LPWSTR lpFileName, LPOFSTRUCT lpReOpenBuf, WORD wStyle)
{
	const int numParams = 3;
	char *functionName = "LZOpenFileW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpFileName };
	void *params[numParams] = { &str[0], &lpReOpenBuf, &wStyle };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, IntegerType };
	
	if (realLZOpenFileW == NULL)
		realLZOpenFileW = (LZOpenFileWFunction)library->GetOriginalFunction(functionName);
		
	if (realLZOpenFileW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		INT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZOpenFileWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "INT", &errorCode, 0, true))
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
// Method:		LZReadReplacement
// Description: See MSDN LZRead function
// Parameters:	See MSDN LZRead parameters
// Return Value: See MSDN LZRead return value
//*************************************************************************
INT APIENTRY LZReadReplacement(INT hFile, LPSTR lpBuffer, INT cbRead)
{
	const int numParams = 3;
	char *functionName = "LZRead";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lpBuffer, &cbRead };
	ParameterType paramTypes[numParams] = { IntegerType, PointerType, IntegerType };
	
	if (realLZRead == NULL)
		realLZRead = (LZReadFunction)library->GetOriginalFunction(functionName);
		
	if (realLZRead != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		INT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZReadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "INT", &errorCode, 0, true))
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
// Method:		LZSeekReplacement
// Description: See MSDN LZSeek function
// Parameters:	See MSDN LZSeek parameters
// Return Value: See MSDN LZSeek return value
//*************************************************************************
LONG APIENTRY LZSeekReplacement(INT hFile, LONG lOffset, INT iOrigin)
{
	const int numParams = 3;
	char *functionName = "LZSeek";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lOffset, &iOrigin };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType, IntegerType };
	
	if (realLZSeek == NULL)
		realLZSeek = (LZSeekFunction)library->GetOriginalFunction(functionName);
		
	if (realLZSeek != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LONG returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LZSeekCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LONG", &errorCode, 0, true))
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
// Method:		LoadModuleReplacement
// Description: See MSDN LoadModule function
// Parameters:	See MSDN LoadModule parameters
// Return Value: See MSDN LoadModule return value
//*************************************************************************
DWORD WINAPI LoadModuleReplacement(LPCSTR lpModuleName, LPVOID lpParameterBlock)
{
	const int numParams = 2;
	char *functionName = "LoadModule";
	char *categoryName = "PROCESS";
	SiString str[] = { (char *)lpModuleName };
	void *params[numParams] = { &str[0], &lpParameterBlock };
	ParameterType paramTypes[numParams] = { StringType, PointerType };
	
	if (realLoadModule == NULL)
		realLoadModule = (LoadModuleFunction)library->GetOriginalFunction(functionName);
		
	if (realLoadModule != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LoadModuleCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		OpenFileReplacement
// Description: See MSDN OpenFile function
// Parameters:	See MSDN OpenFile parameters
// Return Value: See MSDN OpenFile return value
//*************************************************************************
HFILE WINAPI OpenFileReplacement(LPCSTR lpFileName, LPOFSTRUCT lpReOpenBuff, UINT uStyle)
{
	const int numParams = 3;
	char *functionName = "OpenFile";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpFileName };
	void *params[numParams] = { &str[0], &lpReOpenBuff, &uStyle };
	ParameterType paramTypes[numParams] = { StringType, PointerType, IntegerType };
	
	if (realOpenFile == NULL)
		realOpenFile = (OpenFileFunction)library->GetOriginalFunction(functionName);
		
	if (realOpenFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HFILE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, OpenFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HFILE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		library->CreateResourceMapping((HANDLE)returnValue, lpFileName);
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		SetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		QueryMemoryResourceNotificationReplacement
// Description: See MSDN QueryMemoryResourceNotification function
// Parameters:	See MSDN QueryMemoryResourceNotification parameters
// Return Value: See MSDN QueryMemoryResourceNotification return value
//*************************************************************************
BOOL WINAPI QueryMemoryResourceNotificationReplacement(HANDLE ResourceNotificationHandle, PBOOL ResourceState)
{
	const int numParams = 2;
	char *functionName = "QueryMemoryResourceNotification";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &ResourceNotificationHandle, &ResourceState };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realQueryMemoryResourceNotification == NULL)
		realQueryMemoryResourceNotification = (QueryMemoryResourceNotificationFunction)library->GetOriginalFunction(functionName);
		
	if (realQueryMemoryResourceNotification != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, QueryMemoryResourceNotificationCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileApisToANSIReplacement
// Description: See MSDN SetFileApisToANSI function
// Parameters:	See MSDN SetFileApisToANSI parameters
// Return Value: See MSDN SetFileApisToANSI return value
//*************************************************************************
VOID WINAPI SetFileApisToANSIReplacement()
{
	const int numParams = 0;
	char *functionName = "SetFileApisToANSI";
	char *categoryName = "FILE";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realSetFileApisToANSI == NULL)
		realSetFileApisToANSI = (SetFileApisToANSIFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileApisToANSI != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileApisToANSICaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		SetFileApisToOEMReplacement
// Description: See MSDN SetFileApisToOEM function
// Parameters:	See MSDN SetFileApisToOEM parameters
// Return Value: See MSDN SetFileApisToOEM return value
//*************************************************************************
VOID WINAPI SetFileApisToOEMReplacement()
{
	const int numParams = 0;
	char *functionName = "SetFileApisToOEM";
	char *categoryName = "FILE";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realSetFileApisToOEM == NULL)
		realSetFileApisToOEM = (SetFileApisToOEMFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileApisToOEM != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileApisToOEMCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		SetFilePointerReplacement
// Description: See MSDN SetFilePointer function
// Parameters:	See MSDN SetFilePointer parameters
// Return Value: See MSDN SetFilePointer return value
//*************************************************************************
DWORD WINAPI SetFilePointerReplacement(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	const int numParams = 4;
	char *functionName = "SetFilePointer";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lDistanceToMove, &lpDistanceToMoveHigh, &dwMoveMethod };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType, UnsignedLongType };
	
	if (realSetFilePointer == NULL)
		realSetFilePointer = (SetFilePointerFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFilePointer != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFilePointerCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		SetFilePointerExReplacement
// Description: See MSDN SetFilePointerEx function
// Parameters:	See MSDN SetFilePointerEx parameters
// Return Value: See MSDN SetFilePointerEx return value
//*************************************************************************
BOOL WINAPI SetFilePointerExReplacement(HANDLE hFile, LARGE_INTEGER lDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{
	const int numParams = 4;
	char *functionName = "SetFilePointerEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &lDistanceToMove, &lpNewFilePointer, &dwMoveMethod };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType, UnsignedLongType };
	
	if (realSetFilePointerEx == NULL)
		realSetFilePointerEx = (SetFilePointerExFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFilePointerEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFilePointerExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileShortNameAReplacement
// Description: See MSDN SetFileShortNameA function
// Parameters:	See MSDN SetFileShortNameA parameters
// Return Value: See MSDN SetFileShortNameA return value
//*************************************************************************
BOOL WINAPI SetFileShortNameAReplacement(HANDLE hFile, LPCSTR lpShortName)
{
	const int numParams = 2;
	char *functionName = "SetFileShortNameA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpShortName };
	void *params[numParams] = { &hFile, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, StringType };
	
	if (realSetFileShortNameA == NULL)
		realSetFileShortNameA = (SetFileShortNameAFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileShortNameA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileShortNameACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileShortNameWReplacement
// Description: See MSDN SetFileShortNameW function
// Parameters:	See MSDN SetFileShortNameW parameters
// Return Value: See MSDN SetFileShortNameW return value
//*************************************************************************
BOOL WINAPI SetFileShortNameWReplacement(HANDLE hFile, LPCWSTR lpShortName)
{
	const int numParams = 2;
	char *functionName = "SetFileShortNameW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpShortName };
	void *params[numParams] = { &hFile, &str[0] };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType };
	
	if (realSetFileShortNameW == NULL)
		realSetFileShortNameW = (SetFileShortNameWFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileShortNameW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileShortNameWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileValidDataReplacement
// Description: See MSDN SetFileValidData function
// Parameters:	See MSDN SetFileValidData parameters
// Return Value: See MSDN SetFileValidData return value
//*************************************************************************
BOOL WINAPI SetFileValidDataReplacement(HANDLE hFile, LONGLONG ValidDataLength)
{
	const int numParams = 2;
	char *functionName = "SetFileValidData";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &ValidDataLength };
	ParameterType paramTypes[numParams] = { PointerType, Integer64Type };
	
	if (realSetFileValidData == NULL)
		realSetFileValidData = (SetFileValidDataFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileValidData != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileValidDataCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetEndOfFileReplacement
// Description: See MSDN SetEndOfFile function
// Parameters:	See MSDN SetEndOfFile parameters
// Return Value: See MSDN SetEndOfFile return value
//*************************************************************************
BOOL WINAPI SetEndOfFileReplacement(HANDLE hFile)
{
	const int numParams = 1;
	char *functionName = "SetEndOfFile";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realSetEndOfFile == NULL)
		realSetEndOfFile = (SetEndOfFileFunction)library->GetOriginalFunction(functionName);
		
	if (realSetEndOfFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetEndOfFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetPriorityClassReplacement
// Description: See MSDN SetPriorityClass function
// Parameters:	See MSDN SetPriorityClass parameters
// Return Value: See MSDN SetPriorityClass return value
//*************************************************************************
BOOL WINAPI SetPriorityClassReplacement(HANDLE hProcess, DWORD dwPriorityClass)
{
	const int numParams = 2;
	char *functionName = "SetPriorityClass";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hProcess, &dwPriorityClass };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realSetPriorityClass == NULL)
		realSetPriorityClass = (SetPriorityClassFunction)library->GetOriginalFunction(functionName);
		
	if (realSetPriorityClass != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetPriorityClassCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetProcessShutdownParametersReplacement
// Description: See MSDN SetProcessShutdownParameters function
// Parameters:	See MSDN SetProcessShutdownParameters parameters
// Return Value: See MSDN SetProcessShutdownParameters return value
//*************************************************************************
BOOL WINAPI SetProcessShutdownParametersReplacement(DWORD dwLevel, DWORD dwFlags)
{
	const int numParams = 2;
	char *functionName = "SetProcessShutdownParameters";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &dwLevel, &dwFlags };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType };
	
	if (realSetProcessShutdownParameters == NULL)
		realSetProcessShutdownParameters = (SetProcessShutdownParametersFunction)library->GetOriginalFunction(functionName);
		
	if (realSetProcessShutdownParameters != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetProcessShutdownParametersCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetProcessWorkingSetSizeReplacement
// Description: See MSDN SetProcessWorkingSetSize function
// Parameters:	See MSDN SetProcessWorkingSetSize parameters
// Return Value: See MSDN SetProcessWorkingSetSize return value
//*************************************************************************
BOOL WINAPI SetProcessWorkingSetSizeReplacement(HANDLE hProcess, SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize)
{
	const int numParams = 3;
	char *functionName = "SetProcessWorkingSetSize";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &dwMinimumWorkingSetSize, &dwMaximumWorkingSetSize };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realSetProcessWorkingSetSize == NULL)
		realSetProcessWorkingSetSize = (SetProcessWorkingSetSizeFunction)library->GetOriginalFunction(functionName);
		
	if (realSetProcessWorkingSetSize != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetProcessWorkingSetSizeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		TerminateProcessReplacement
// Description: See MSDN TerminateProcess function
// Parameters:	See MSDN TerminateProcess parameters
// Return Value: See MSDN TerminateProcess return value
//*************************************************************************
BOOL WINAPI TerminateProcessReplacement(HANDLE hProcess, UINT uExitCode)
{
	const int numParams = 2;
	char *functionName = "TerminateProcess";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hProcess, &uExitCode };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realTerminateProcess == NULL)
		realTerminateProcess = (TerminateProcessFunction)library->GetOriginalFunction(functionName);
		
	if (realTerminateProcess != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, TerminateProcessCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		TerminateThreadReplacement
// Description: See MSDN TerminateThread function
// Parameters:	See MSDN TerminateThread parameters
// Return Value: See MSDN TerminateThread return value
//*************************************************************************
BOOL WINAPI TerminateThreadReplacement(HANDLE hThread, DWORD dwExitCode)
{
	const int numParams = 2;
	char *functionName = "TerminateThread";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &hThread, &dwExitCode };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };
	
	if (realTerminateThread == NULL)
		realTerminateThread = (TerminateThreadFunction)library->GetOriginalFunction(functionName);
		
	if (realTerminateThread != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, TerminateThreadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		TlsAllocReplacement
// Description: See MSDN TlsAlloc function
// Parameters:	See MSDN TlsAlloc parameters
// Return Value: See MSDN TlsAlloc return value
//*************************************************************************
DWORD WINAPI TlsAllocReplacement()
{
	const int numParams = 0;
	char *functionName = "TlsAlloc";
	char *categoryName = "MEMORY";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realTlsAlloc == NULL)
		realTlsAlloc = (TlsAllocFunction)library->GetOriginalFunction(functionName);
		
	if (realTlsAlloc != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, TlsAllocCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		TlsFreeReplacement
// Description: See MSDN TlsFree function
// Parameters:	See MSDN TlsFree parameters
// Return Value: See MSDN TlsFree return value
//*************************************************************************
BOOL WINAPI TlsFreeReplacement(DWORD dwTlsIndex)
{
	const int numParams = 1;
	char *functionName = "TlsFree";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &dwTlsIndex };
	ParameterType paramTypes[numParams] = { UnsignedLongType };
	
	if (realTlsFree == NULL)
		realTlsFree = (TlsFreeFunction)library->GetOriginalFunction(functionName);
		
	if (realTlsFree != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, TlsFreeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		UnlockFileReplacement
// Description: See MSDN UnlockFile function
// Parameters:	See MSDN UnlockFile parameters
// Return Value: See MSDN UnlockFile return value
//*************************************************************************
BOOL WINAPI UnlockFileReplacement(HANDLE hFile, DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh, DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh)
{
	const int numParams = 5;
	char *functionName = "UnlockFile";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &dwFileOffsetLow, &dwFileOffsetHigh, &nNumberOfBytesToUnlockLow, &nNumberOfBytesToUnlockHigh };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, UnsignedLongType };
	
	if (realUnlockFile == NULL)
		realUnlockFile = (UnlockFileFunction)library->GetOriginalFunction(functionName);
		
	if (realUnlockFile != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, UnlockFileCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		UnlockFileExReplacement
// Description: See MSDN UnlockFileEx function
// Parameters:	See MSDN UnlockFileEx parameters
// Return Value: See MSDN UnlockFileEx return value
//*************************************************************************
BOOL WINAPI UnlockFileExReplacement(HANDLE hFile, DWORD dwReserved, DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 5;
	char *functionName = "UnlockFileEx";
	char *categoryName = "FILE";
	
	void *params[numParams] = { &hFile, &dwReserved, &nNumberOfBytesToUnlockLow, &nNumberOfBytesToUnlockHigh, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realUnlockFileEx == NULL)
		realUnlockFileEx = (UnlockFileExFunction)library->GetOriginalFunction(functionName);
		
	if (realUnlockFileEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, UnlockFileExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualProtectReplacement
// Description: See MSDN VirtualProtect function
// Parameters:	See MSDN VirtualProtect parameters
// Return Value: See MSDN VirtualProtect return value
//*************************************************************************
BOOL WINAPI VirtualProtectReplacement(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
	const int numParams = 4;
	char *functionName = "VirtualProtect";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &lpAddress, &dwSize, &flNewProtect, &lpflOldProtect };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realVirtualProtect == NULL)
		realVirtualProtect = (VirtualProtectFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualProtect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualProtectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		VirtualProtectExReplacement
// Description: See MSDN VirtualProtectEx function
// Parameters:	See MSDN VirtualProtectEx parameters
// Return Value: See MSDN VirtualProtectEx return value
//*************************************************************************
BOOL WINAPI VirtualProtectExReplacement(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
	const int numParams = 5;
	char *functionName = "VirtualProtectEx";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpAddress, &dwSize, &flNewProtect, &lpflOldProtect };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, UnsignedLongType, PointerType };
	
	if (realVirtualProtectEx == NULL)
		realVirtualProtectEx = (VirtualProtectExFunction)library->GetOriginalFunction(functionName);
		
	if (realVirtualProtectEx != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, VirtualProtectExCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		WriteProcessMemoryReplacement
// Description: See MSDN WriteProcessMemory function
// Parameters:	See MSDN WriteProcessMemory parameters
// Return Value: See MSDN WriteProcessMemory return value
//*************************************************************************
BOOL WINAPI WriteProcessMemoryReplacement(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten)
{
	const int numParams = 5;
	char *functionName = "WriteProcessMemory";
	char *categoryName = "MEMORY";
	
	void *params[numParams] = { &hProcess, &lpBaseAddress, &lpBuffer, &nSize, &lpNumberOfBytesWritten };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, UnsignedLongType, PointerType };
	
	if (realWriteProcessMemory == NULL)
		realWriteProcessMemory = (WriteProcessMemoryFunction)library->GetOriginalFunction(functionName);
		
	if (realWriteProcessMemory != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WriteProcessMemoryCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CompareFileTimeReplacement
// Description: See MSDN CompareFileTime function
// Parameters:	See MSDN CompareFileTime parameters
// Return Value: See MSDN CompareFileTime return value
//*************************************************************************
LONG WINAPI CompareFileTimeReplacement(CONST FILETIME * lpFileTime1, CONST FILETIME * lpFileTime2)
{
	const int numParams = 2;
	char *functionName = "CompareFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpFileTime1, &lpFileTime2 };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realCompareFileTime == NULL)
		realCompareFileTime = (CompareFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realCompareFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LONG returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CompareFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LONG", &errorCode, 0, true))
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
// Method:		DosDateTimeToFileTimeReplacement
// Description: See MSDN DosDateTimeToFileTime function
// Parameters:	See MSDN DosDateTimeToFileTime parameters
// Return Value: See MSDN DosDateTimeToFileTime return value
//*************************************************************************
BOOL WINAPI DosDateTimeToFileTimeReplacement(WORD wFatDate, WORD wFatTime, LPFILETIME lpFileTime)
{
	const int numParams = 3;
	char *functionName = "DosDateTimeToFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &wFatDate, &wFatTime, &lpFileTime };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType, PointerType };
	
	if (realDosDateTimeToFileTime == NULL)
		realDosDateTimeToFileTime = (DosDateTimeToFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realDosDateTimeToFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DosDateTimeToFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FileTimeToDosDateTimeReplacement
// Description: See MSDN FileTimeToDosDateTime function
// Parameters:	See MSDN FileTimeToDosDateTime parameters
// Return Value: See MSDN FileTimeToDosDateTime return value
//*************************************************************************
BOOL WINAPI FileTimeToDosDateTimeReplacement(CONST FILETIME * lpFileTime, LPWORD lpFatDate, LPWORD lpFatTime)
{
	const int numParams = 3;
	char *functionName = "FileTimeToDosDateTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpFileTime, &lpFatDate, &lpFatTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realFileTimeToDosDateTime == NULL)
		realFileTimeToDosDateTime = (FileTimeToDosDateTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realFileTimeToDosDateTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FileTimeToDosDateTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FileTimeToLocalFileTimeReplacement
// Description: See MSDN FileTimeToLocalFileTime function
// Parameters:	See MSDN FileTimeToLocalFileTime parameters
// Return Value: See MSDN FileTimeToLocalFileTime return value
//*************************************************************************
BOOL WINAPI FileTimeToLocalFileTimeReplacement(CONST FILETIME * lpFileTime, LPFILETIME lpLocalFileTime)
{
	const int numParams = 2;
	char *functionName = "FileTimeToLocalFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpFileTime, &lpLocalFileTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realFileTimeToLocalFileTime == NULL)
		realFileTimeToLocalFileTime = (FileTimeToLocalFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realFileTimeToLocalFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FileTimeToLocalFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		FileTimeToSystemTimeReplacement
// Description: See MSDN FileTimeToSystemTime function
// Parameters:	See MSDN FileTimeToSystemTime parameters
// Return Value: See MSDN FileTimeToSystemTime return value
//*************************************************************************
BOOL WINAPI FileTimeToSystemTimeReplacement(CONST FILETIME * lpFileTime, LPSYSTEMTIME lpSystemTime)
{
	const int numParams = 2;
	char *functionName = "FileTimeToSystemTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpFileTime, &lpSystemTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realFileTimeToSystemTime == NULL)
		realFileTimeToSystemTime = (FileTimeToSystemTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realFileTimeToSystemTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, FileTimeToSystemTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetFileTimeReplacement
// Description: See MSDN GetFileTime function
// Parameters:	See MSDN GetFileTime parameters
// Return Value: See MSDN GetFileTime return value
//*************************************************************************
BOOL WINAPI GetFileTimeReplacement(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
{
	const int numParams = 4;
	char *functionName = "GetFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &hFile, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, PointerType };
	
	if (realGetFileTime == NULL)
		realGetFileTime = (GetFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, GetFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetLocalTimeReplacement
// Description: See MSDN GetLocalTime function
// Parameters:	See MSDN GetLocalTime parameters
// Return Value: See MSDN GetLocalTime return value
//*************************************************************************
VOID WINAPI GetLocalTimeReplacement(LPSYSTEMTIME lpSystemTime)
{
	const int numParams = 1;
	char *functionName = "GetLocalTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTime };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetLocalTime == NULL)
		realGetLocalTime = (GetLocalTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetLocalTime != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetLocalTimeCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetSystemTimeReplacement
// Description: See MSDN GetSystemTime function
// Parameters:	See MSDN GetSystemTime parameters
// Return Value: See MSDN GetSystemTime return value
//*************************************************************************
VOID WINAPI GetSystemTimeReplacement(LPSYSTEMTIME lpSystemTime)
{
	const int numParams = 1;
	char *functionName = "GetSystemTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTime };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetSystemTime == NULL)
		realGetSystemTime = (GetSystemTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemTime != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemTimeCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetSystemTimeAdjustmentReplacement
// Description: See MSDN GetSystemTimeAdjustment function
// Parameters:	See MSDN GetSystemTimeAdjustment parameters
// Return Value: See MSDN GetSystemTimeAdjustment return value
//*************************************************************************
BOOL WINAPI GetSystemTimeAdjustmentReplacement(PDWORD lpTimeAdjustment, PDWORD lpTimeIncrement, PBOOL lpTimeAdjustmentDisabled)
{
	const int numParams = 3;
	char *functionName = "GetSystemTimeAdjustment";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpTimeAdjustment, &lpTimeIncrement, &lpTimeAdjustmentDisabled };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realGetSystemTimeAdjustment == NULL)
		realGetSystemTimeAdjustment = (GetSystemTimeAdjustmentFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemTimeAdjustment != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemTimeAdjustmentCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetSystemTimeAsFileTimeReplacement
// Description: See MSDN GetSystemTimeAsFileTime function
// Parameters:	See MSDN GetSystemTimeAsFileTime parameters
// Return Value: See MSDN GetSystemTimeAsFileTime return value
//*************************************************************************
VOID WINAPI GetSystemTimeAsFileTimeReplacement(LPFILETIME lpSystemTimeAsFileTime)
{
	const int numParams = 1;
	char *functionName = "GetSystemTimeAsFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTimeAsFileTime };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetSystemTimeAsFileTime == NULL)
		realGetSystemTimeAsFileTime = (GetSystemTimeAsFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemTimeAsFileTime != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemTimeAsFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetTimeFormatAReplacement
// Description: See MSDN GetTimeFormatA function
// Parameters:	See MSDN GetTimeFormatA parameters
// Return Value: See MSDN GetTimeFormatA return value
//*************************************************************************
int WINAPI GetTimeFormatAReplacement(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME * lpTime, LPCSTR lpFormat, LPSTR lpTimeStr, int cchTime)
{
	const int numParams = 6;
	char *functionName = "GetTimeFormatA";
	char *categoryName = "TIME";
	SiString str[] = { (char *)lpFormat };
	void *params[numParams] = { &Locale, &dwFlags, &lpTime, &str[0], &lpTimeStr, &cchTime };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, PointerType, StringType, PointerType, IntegerType };
	
	if (realGetTimeFormatA == NULL)
		realGetTimeFormatA = (GetTimeFormatAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetTimeFormatA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetTimeFormatACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
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
// Method:		GetTimeFormatWReplacement
// Description: See MSDN GetTimeFormatW function
// Parameters:	See MSDN GetTimeFormatW parameters
// Return Value: See MSDN GetTimeFormatW return value
//*************************************************************************
int WINAPI GetTimeFormatWReplacement(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME * lpTime, LPCWSTR lpFormat, LPWSTR lpTimeStr, int cchTime)
{
	const int numParams = 6;
	char *functionName = "GetTimeFormatW";
	char *categoryName = "TIME";
	SiString str[] = { (wchar_t *)lpFormat };
	void *params[numParams] = { &Locale, &dwFlags, &lpTime, &str[0], &lpTimeStr, &cchTime };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, PointerType, WideStringType, PointerType, IntegerType };
	
	if (realGetTimeFormatW == NULL)
		realGetTimeFormatW = (GetTimeFormatWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetTimeFormatW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetTimeFormatWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
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
// Method:		GetTimeZoneInformationReplacement
// Description: See MSDN GetTimeZoneInformation function
// Parameters:	See MSDN GetTimeZoneInformation parameters
// Return Value: See MSDN GetTimeZoneInformation return value
//*************************************************************************
DWORD WINAPI GetTimeZoneInformationReplacement(LPTIME_ZONE_INFORMATION lpTimeZoneInformation)
{
	const int numParams = 1;
	char *functionName = "GetTimeZoneInformation";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpTimeZoneInformation };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetTimeZoneInformation == NULL)
		realGetTimeZoneInformation = (GetTimeZoneInformationFunction)library->GetOriginalFunction(functionName);
		
	if (realGetTimeZoneInformation != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		DWORD returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetTimeZoneInformationCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "DWORD", &errorCode, 0, true))
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
// Method:		LocalFileTimeToFileTimeReplacement
// Description: See MSDN LocalFileTimeToFileTime function
// Parameters:	See MSDN LocalFileTimeToFileTime parameters
// Return Value: See MSDN LocalFileTimeToFileTime return value
//*************************************************************************
BOOL WINAPI LocalFileTimeToFileTimeReplacement(CONST FILETIME * lpLocalFileTime, LPFILETIME lpFileTime)
{
	const int numParams = 2;
	char *functionName = "LocalFileTimeToFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpLocalFileTime, &lpFileTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realLocalFileTimeToFileTime == NULL)
		realLocalFileTimeToFileTime = (LocalFileTimeToFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realLocalFileTimeToFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LocalFileTimeToFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetFileTimeReplacement
// Description: See MSDN SetFileTime function
// Parameters:	See MSDN SetFileTime parameters
// Return Value: See MSDN SetFileTime return value
//*************************************************************************
BOOL WINAPI SetFileTimeReplacement(HANDLE hFile, CONST FILETIME * lpCreationTime, CONST FILETIME * lpLastAccessTime, CONST FILETIME * lpLastWriteTime)
{
	const int numParams = 4;
	char *functionName = "SetFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &hFile, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, PointerType };
	
	if (realSetFileTime == NULL)
		realSetFileTime = (SetFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realSetFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		//add to pathlist for use by doestestmatch
		library->AddToPathList(0, library->GetResourceMapping(hFile));
		
		if (library->RunStandardTestsAndGetResults(logSender, SetFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetLocalTimeReplacement
// Description: See MSDN SetLocalTime function
// Parameters:	See MSDN SetLocalTime parameters
// Return Value: See MSDN SetLocalTime return value
//*************************************************************************
BOOL WINAPI SetLocalTimeReplacement(CONST SYSTEMTIME * lpSystemTime)
{
	const int numParams = 1;
	char *functionName = "SetLocalTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTime };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realSetLocalTime == NULL)
		realSetLocalTime = (SetLocalTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realSetLocalTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetLocalTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetSystemTimeReplacement
// Description: See MSDN SetSystemTime function
// Parameters:	See MSDN SetSystemTime parameters
// Return Value: See MSDN SetSystemTime return value
//*************************************************************************
BOOL WINAPI SetSystemTimeReplacement(CONST SYSTEMTIME * lpSystemTime)
{
	const int numParams = 1;
	char *functionName = "SetSystemTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTime };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realSetSystemTime == NULL)
		realSetSystemTime = (SetSystemTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realSetSystemTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetSystemTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetSystemTimeAdjustmentReplacement
// Description: See MSDN SetSystemTimeAdjustment function
// Parameters:	See MSDN SetSystemTimeAdjustment parameters
// Return Value: See MSDN SetSystemTimeAdjustment return value
//*************************************************************************
BOOL WINAPI SetSystemTimeAdjustmentReplacement(DWORD dwTimeAdjustment, BOOL bTimeAdjustmentDisabled)
{
	const int numParams = 2;
	char *functionName = "SetSystemTimeAdjustment";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &dwTimeAdjustment, &bTimeAdjustmentDisabled };
	ParameterType paramTypes[numParams] = { UnsignedLongType, BooleanType };
	
	if (realSetSystemTimeAdjustment == NULL)
		realSetSystemTimeAdjustment = (SetSystemTimeAdjustmentFunction)library->GetOriginalFunction(functionName);
		
	if (realSetSystemTimeAdjustment != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetSystemTimeAdjustmentCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SetTimeZoneInformationReplacement
// Description: See MSDN SetTimeZoneInformation function
// Parameters:	See MSDN SetTimeZoneInformation parameters
// Return Value: See MSDN SetTimeZoneInformation return value
//*************************************************************************
BOOL WINAPI SetTimeZoneInformationReplacement(CONST TIME_ZONE_INFORMATION * lpTimeZoneInformation)
{
	const int numParams = 1;
	char *functionName = "SetTimeZoneInformation";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpTimeZoneInformation };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realSetTimeZoneInformation == NULL)
		realSetTimeZoneInformation = (SetTimeZoneInformationFunction)library->GetOriginalFunction(functionName);
		
	if (realSetTimeZoneInformation != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SetTimeZoneInformationCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SystemTimeToFileTimeReplacement
// Description: See MSDN SystemTimeToFileTime function
// Parameters:	See MSDN SystemTimeToFileTime parameters
// Return Value: See MSDN SystemTimeToFileTime return value
//*************************************************************************
BOOL WINAPI SystemTimeToFileTimeReplacement(CONST SYSTEMTIME * lpSystemTime, LPFILETIME lpFileTime)
{
	const int numParams = 2;
	char *functionName = "SystemTimeToFileTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpSystemTime, &lpFileTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realSystemTimeToFileTime == NULL)
		realSystemTimeToFileTime = (SystemTimeToFileTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realSystemTimeToFileTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SystemTimeToFileTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		SystemTimeToTzSpecificLocalTimeReplacement
// Description: See MSDN SystemTimeToTzSpecificLocalTime function
// Parameters:	See MSDN SystemTimeToTzSpecificLocalTime parameters
// Return Value: See MSDN SystemTimeToTzSpecificLocalTime return value
//*************************************************************************
BOOL WINAPI SystemTimeToTzSpecificLocalTimeReplacement(LPTIME_ZONE_INFORMATION lpTimeZoneInformation, LPSYSTEMTIME lpUniversalTime, LPSYSTEMTIME lpLocalTime)
{
	const int numParams = 3;
	char *functionName = "SystemTimeToTzSpecificLocalTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpTimeZoneInformation, &lpUniversalTime, &lpLocalTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realSystemTimeToTzSpecificLocalTime == NULL)
		realSystemTimeToTzSpecificLocalTime = (SystemTimeToTzSpecificLocalTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realSystemTimeToTzSpecificLocalTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, SystemTimeToTzSpecificLocalTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		TzSpecificLocalTimeToSystemTimeReplacement
// Description: See MSDN TzSpecificLocalTimeToSystemTime function
// Parameters:	See MSDN TzSpecificLocalTimeToSystemTime parameters
// Return Value: See MSDN TzSpecificLocalTimeToSystemTime return value
//*************************************************************************
BOOL WINAPI TzSpecificLocalTimeToSystemTimeReplacement(LPTIME_ZONE_INFORMATION lpTimeZoneInformation, LPSYSTEMTIME lpLocalTime, LPSYSTEMTIME lpUniversalTime)
{
	const int numParams = 3;
	char *functionName = "TzSpecificLocalTimeToSystemTime";
	char *categoryName = "TIME";
	
	void *params[numParams] = { &lpTimeZoneInformation, &lpLocalTime, &lpUniversalTime };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realTzSpecificLocalTimeToSystemTime == NULL)
		realTzSpecificLocalTimeToSystemTime = (TzSpecificLocalTimeToSystemTimeFunction)library->GetOriginalFunction(functionName);
		
	if (realTzSpecificLocalTimeToSystemTime != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, TzSpecificLocalTimeToSystemTimeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		QueryPerformanceCounterReplacement
// Description: See MSDN QueryPerformanceCounter function
// Parameters:	See MSDN QueryPerformanceCounter parameters
// Return Value: See MSDN QueryPerformanceCounter return value
//*************************************************************************
BOOL WINAPI QueryPerformanceCounterReplacement(LARGE_INTEGER * lpPerformanceCount)
{
	const int numParams = 1;
	char *functionName = "QueryPerformanceCounter";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpPerformanceCount };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realQueryPerformanceCounter == NULL)
		realQueryPerformanceCounter = (QueryPerformanceCounterFunction)library->GetOriginalFunction(functionName);
		
	if (realQueryPerformanceCounter != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, QueryPerformanceCounterCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		QueryPerformanceFrequencyReplacement
// Description: See MSDN QueryPerformanceFrequency function
// Parameters:	See MSDN QueryPerformanceFrequency parameters
// Return Value: See MSDN QueryPerformanceFrequency return value
//*************************************************************************
BOOL WINAPI QueryPerformanceFrequencyReplacement(LARGE_INTEGER * lpFrequency)
{
	const int numParams = 1;
	char *functionName = "QueryPerformanceFrequency";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpFrequency };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realQueryPerformanceFrequency == NULL)
		realQueryPerformanceFrequency = (QueryPerformanceFrequencyFunction)library->GetOriginalFunction(functionName);
		
	if (realQueryPerformanceFrequency != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, QueryPerformanceFrequencyCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method: GetProcAddressReplacement
// Description: See MSDN GetProcAddress function
// Parameters:	See MSDN GetProcAddress parameters
// Return Value: See MSDN GetProcAddress return value
//*************************************************************************
FARPROC WINAPI GetProcAddressReplacement(HMODULE hModule, LPCSTR lpProcName)
{
	// if the high word is zero, lpProcName is not a pointer, it is an ordinal
	if ((((DWORD)lpProcName) & 0xFFFF0000) != 0)
	{
		const int numParams = 2;
		char *functionName = "GetProcAddress";
		char *categoryName = "MEMORY";	
		SiString str = (char *)lpProcName;
		void *params[numParams] = { &hModule, &str };
		ParameterType paramTypes[numParams] = { PointerType, StringType };

		if (realGetProcAddress == NULL)
			realGetProcAddress = (GetProcAddressFunction)library->GetOriginalFunction(functionName);

		if (realGetProcAddress != NULL)
		{
			DWORD errorCode, tempReturnValue;
			FARPROC returnValue;
			library->RunStandardTestsAndGetResults(logSender, GetProcAddressCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "FARPROC", &errorCode, 0, true);
			memcpy (&returnValue, &tempReturnValue, sizeof(DWORD));
			SetLastError(errorCode);
			return returnValue;
		}

		return 0;
	}
	else
	{
		const int numParams = 2;
		char *functionName = "GetProcAddress";
		char *categoryName = "MEMORY";	
		void *params[numParams] = { &hModule, &lpProcName };
		ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType };

		if (realGetProcAddress == NULL)
			realGetProcAddress = (GetProcAddressFunction)library->GetOriginalFunction(functionName);

		if (realGetProcAddress != NULL)
		{
			DWORD errorCode, tempReturnValue;
			FARPROC returnValue;
			library->RunStandardTestsAndGetResults(logSender, GetProcAddressCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "FARPROC", &errorCode, 0, true);
			memcpy (&returnValue, &tempReturnValue, sizeof(DWORD));
			SetLastError(errorCode);
			return returnValue;
		}

		return 0;
	}
}
//*************************************************************************
// Method:		GetModuleHandleAReplacement
// Description: See MSDN GetModuleHandleA function
// Parameters:	See MSDN GetModuleHandleA parameters
// Return Value: See MSDN GetModuleHandleA return value
//*************************************************************************
HMODULE WINAPI GetModuleHandleAReplacement(LPCSTR lpModuleName)
{
	const int numParams = 1;
	char *functionName = "GetModuleHandleA";
	char *categoryName = "LIBRARY";
	SiString str[] = { (char *)lpModuleName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { StringType };
	
	if (realGetModuleHandleA == NULL)
		realGetModuleHandleA = (GetModuleHandleAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetModuleHandleA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetModuleHandleACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		GetModuleHandleWReplacement
// Description: See MSDN GetModuleHandleW function
// Parameters:	See MSDN GetModuleHandleW parameters
// Return Value: See MSDN GetModuleHandleW return value
//*************************************************************************
HMODULE WINAPI GetModuleHandleWReplacement(LPCWSTR lpModuleName)
{
	const int numParams = 1;
	char *functionName = "GetModuleHandleW";
	char *categoryName = "LIBRARY";
	SiString str[] = { (wchar_t *)lpModuleName };
	void *params[numParams] = { &str[0] };
	ParameterType paramTypes[numParams] = { WideStringType };
	
	if (realGetModuleHandleW == NULL)
		realGetModuleHandleW = (GetModuleHandleWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetModuleHandleW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HMODULE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetModuleHandleWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HMODULE", &errorCode, 0, true))
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
// Method:		DeviceIoControlReplacement
// Description: See MSDN DeviceIoControl function
// Parameters:	See MSDN DeviceIoControl parameters
// Return Value: See MSDN DeviceIoControl return value
//*************************************************************************
BOOL WINAPI DeviceIoControlReplacement(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 8;
	char *functionName = "DeviceIoControl";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &hDevice, &dwIoControlCode, &lpInBuffer, &nInBufferSize, &lpOutBuffer, &nOutBufferSize, &lpBytesReturned, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realDeviceIoControl == NULL)
		realDeviceIoControl = (DeviceIoControlFunction)library->GetOriginalFunction(functionName);
		
	if (realDeviceIoControl != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DeviceIoControlCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetVersionExAReplacement
// Description: See MSDN GetVersionExA function
// Parameters:	See MSDN GetVersionExA parameters
// Return Value: See MSDN GetVersionExA return value
//*************************************************************************
BOOL WINAPI GetVersionExAReplacement(LPOSVERSIONINFOA lpVersionInformation)
{
	const int numParams = 1;
	char *functionName = "GetVersionExA";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &lpVersionInformation };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetVersionExA == NULL)
		realGetVersionExA = (GetVersionExAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetVersionExA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetVersionExACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetVersionExWReplacement
// Description: See MSDN GetVersionExW function
// Parameters:	See MSDN GetVersionExW parameters
// Return Value: See MSDN GetVersionExW return value
//*************************************************************************
BOOL WINAPI GetVersionExWReplacement(LPOSVERSIONINFOW lpVersionInformation)
{
	const int numParams = 1;
	char *functionName = "GetVersionExW";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &lpVersionInformation };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetVersionExW == NULL)
		realGetVersionExW = (GetVersionExWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetVersionExW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetVersionExWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetSystemInfoReplacement
// Description: See MSDN GetSystemInfo function
// Parameters:	See MSDN GetSystemInfo parameters
// Return Value: See MSDN GetSystemInfo return value
//*************************************************************************
VOID WINAPI GetSystemInfoReplacement(LPSYSTEM_INFO lpSystemInfo)
{
	const int numParams = 1;
	char *functionName = "GetSystemInfo";
	char *categoryName = "MISCELLANEOUS";
	
	void *params[numParams] = { &lpSystemInfo };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realGetSystemInfo == NULL)
		realGetSystemInfo = (GetSystemInfoFunction)library->GetOriginalFunction(functionName);
		
	if (realGetSystemInfo != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetSystemInfoCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		GetVolumeInformationAReplacement
// Description: See MSDN GetVolumeInformationA function
// Parameters:	See MSDN GetVolumeInformationA parameters
// Return Value: See MSDN GetVolumeInformationA return value
//*************************************************************************
BOOL WINAPI GetVolumeInformationAReplacement(LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
	const int numParams = 8;
	char *functionName = "GetVolumeInformationA";
	char *categoryName = "FILE";
	SiString str[] = { (char *)lpRootPathName };
	void *params[numParams] = { &str[0], &lpVolumeNameBuffer, &nVolumeNameSize, &lpVolumeSerialNumber, &lpMaximumComponentLength, &lpFileSystemFlags, &lpFileSystemNameBuffer, &nFileSystemNameSize };
	ParameterType paramTypes[numParams] = { StringType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realGetVolumeInformationA == NULL)
		realGetVolumeInformationA = (GetVolumeInformationAFunction)library->GetOriginalFunction(functionName);
		
	if (realGetVolumeInformationA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetVolumeInformationACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		GetVolumeInformationWReplacement
// Description: See MSDN GetVolumeInformationW function
// Parameters:	See MSDN GetVolumeInformationW parameters
// Return Value: See MSDN GetVolumeInformationW return value
//*************************************************************************
BOOL WINAPI GetVolumeInformationWReplacement(LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer, DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
	const int numParams = 8;
	char *functionName = "GetVolumeInformationW";
	char *categoryName = "FILE";
	SiString str[] = { (wchar_t *)lpRootPathName };
	void *params[numParams] = { &str[0], &lpVolumeNameBuffer, &nVolumeNameSize, &lpVolumeSerialNumber, &lpMaximumComponentLength, &lpFileSystemFlags, &lpFileSystemNameBuffer, &nFileSystemNameSize };
	ParameterType paramTypes[numParams] = { WideStringType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realGetVolumeInformationW == NULL)
		realGetVolumeInformationW = (GetVolumeInformationWFunction)library->GetOriginalFunction(functionName);
		
	if (realGetVolumeInformationW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetVolumeInformationWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		CreateRemoteThreadReplacement
// Description: See MSDN CreateRemoteThread function
// Parameters:	See MSDN CreateRemoteThread parameters
// Return Value: See MSDN CreateRemoteThread return value
//*************************************************************************
HANDLE WINAPI CreateRemoteThreadReplacement(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	createProcessMutex->Acquire();

	DWORD threadID = 0;
	LPDWORD pThreadID = &threadID;
	const int numParams = 7;
	char *functionName = "CreateRemoteThread";
	char *categoryName = "PROCESS";

	if (lpThreadId)
		*pThreadID = *lpThreadId;
	else
		*pThreadID = 0;
	
	void *params[numParams] = { &hProcess, &lpThreadAttributes, &dwStackSize, &lpStartAddress, &lpParameter, &dwCreationFlags, &pThreadID };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType, UnsignedLongType, PointerType };
	
	if (realCreateRemoteThread == NULL)
		realCreateRemoteThread = (CreateRemoteThreadFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateRemoteThread != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		library->RunStandardTestsAndGetResults(logSender, CreateRemoteThreadCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true);
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));

		if ((hProcess == GetCurrentProcess()) && (!library->IsAutCreatedThread(*pThreadID)))
		{
			//send the type flag
			SiString fileString = NEW_THREAD_FLAG;
			fileString += " ";
			// write the thread id
			char tidStr[10];
			sprintf(tidStr, "%d", *pThreadID);
			fileString += tidStr;
			fileString += " ";
			// write the resume app flag
			fileString += ((dwCreationFlags & CREATE_SUSPENDED) ? "0" : "1");
			childProcessDataFile->Write(fileString, strlen(fileString) + 1);

			// tell holodeck the process was created
			childProcessCreatedEvent->Signal();

			// wait for holodeck to be done processing
			holodeckAttachedToChildProcessEvent->Acquire();
		}
		
		if (lpThreadId)
		{
			*lpThreadId = *pThreadID;
		}
		createProcessMutex->Release();

		SetLastError(errorCode);
		return returnValue;
	}
	
	createProcessMutex->Release();
	return 0;
}
//*************************************************************************
// Method:		lstrcpyAReplacement
// Description: See MSDN lstrcpyA function
// Parameters:	See MSDN lstrcpyA parameters
// Return Value: See MSDN lstrcpyA return value
//*************************************************************************
LPSTR WINAPI lstrcpyAReplacement(LPSTR lpString1, LPSTR lpString2)
{
	const int numParams = 2;
	char *functionName = "lstrcpyA";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0] };
	ParameterType paramTypes[numParams] = { OutStringType, StringType };
	
	if (reallstrcpyA == NULL)
		reallstrcpyA = (lstrcpyAFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcpyA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcpyACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPSTR", &errorCode, 0, true))
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
// Method:		lstrcatAReplacement
// Description: See MSDN lstrcatA function
// Parameters:	See MSDN lstrcatA parameters
// Return Value: See MSDN lstrcatA return value
//*************************************************************************
LPSTR WINAPI lstrcatAReplacement(LPSTR lpString1, LPSTR lpString2)
{
	const int numParams = 2;
	char *functionName = "lstrcatA";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0] };
	ParameterType paramTypes[numParams] = { OutStringType, StringType };
	
	if (reallstrcatA == NULL)
		reallstrcatA = (lstrcatAFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcatA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcatACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPSTR", &errorCode, 0, true))
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
// Method:		lstrcpynAReplacement
// Description: See MSDN lstrcpynA function
// Parameters:	See MSDN lstrcpynA parameters
// Return Value: See MSDN lstrcpynA return value
//*************************************************************************
LPSTR WINAPI lstrcpynAReplacement(LPSTR lpString1, LPCSTR lpString2, int iMaxLength)
{
	const int numParams = 3;
	char *functionName = "lstrcpynA";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0], &iMaxLength };
	ParameterType paramTypes[numParams] = { PointerType, StringType, IntegerType };
	
	if (reallstrcpynA == NULL)
		reallstrcpynA = (lstrcpynAFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcpynA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcpynACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPSTR", &errorCode, 0, true))
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
// Method:		MultiByteToWideCharReplacement
// Description: See MSDN MultiByteToWideChar function
// Parameters:	See MSDN MultiByteToWideChar parameters
// Return Value: See MSDN MultiByteToWideChar return value
//*************************************************************************
int WINAPI MultiByteToWideCharReplacement(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	const int numParams = 6;
	char *functionName = "MultiByteToWideChar";
	char *categoryName = "DANGEROUS";
	SiString str;
	void *params[numParams] = { &CodePage, &dwFlags, &lpMultiByteStr, &cbMultiByte, &lpWideCharStr, &cchWideChar };
	ParameterType paramTypes[numParams] = { UnsignedLongType, UnsignedLongType, PointerType, IntegerType, PointerType, IntegerType };

	if (cbMultiByte == -1)
	{
		// String is null terminated, so use an SiString to display it to the user
		str = (char *)lpMultiByteStr;
		params[2] = &str;
		paramTypes[2] = StringType;
	}

	if (realMultiByteToWideChar == NULL)
		realMultiByteToWideChar = (MultiByteToWideCharFunction)library->GetOriginalFunction(functionName);
		
	if (realMultiByteToWideChar != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, MultiByteToWideCharCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
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
// Method:		WinExecReplacement
// Description: See MSDN WinExec function
// Parameters:	See MSDN WinExec parameters
// Return Value: See MSDN WinExec return value
//*************************************************************************
UINT WINAPI WinExecReplacement(LPCSTR lpCmdLine, UINT uCmdShow)
{
	const int numParams = 2;
	char *functionName = "WinExec";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (char *)lpCmdLine };
	void *params[numParams] = { &str[0], &uCmdShow };
	ParameterType paramTypes[numParams] = { StringType, UnsignedLongType };
	
	if (realWinExec == NULL)
		realWinExec = (WinExecFunction)library->GetOriginalFunction(functionName);
		
	if (realWinExec != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		UINT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WinExecCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "UINT", &errorCode, 0, true))
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
// Method:		InitializeCriticalSectionReplacement
// Description: See MSDN InitializeCriticalSection function
// Parameters:	See MSDN InitializeCriticalSection parameters
// Return Value: See MSDN InitializeCriticalSection return value
//*************************************************************************
VOID WINAPI InitializeCriticalSectionReplacement(LPCRITICAL_SECTION lpCriticalSection)
{
	const int numParams = 1;
	char *functionName = "InitializeCriticalSection";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &lpCriticalSection };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realInitializeCriticalSection == NULL)
		realInitializeCriticalSection = (InitializeCriticalSectionFunction)library->GetOriginalFunction(functionName);
		
	if (realInitializeCriticalSection != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, InitializeCriticalSectionCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		EnterCriticalSectionReplacement
// Description: See MSDN EnterCriticalSection function
// Parameters:	See MSDN EnterCriticalSection parameters
// Return Value: See MSDN EnterCriticalSection return value
//*************************************************************************
VOID WINAPI EnterCriticalSectionReplacement(LPCRITICAL_SECTION lpCriticalSection)
{
	const int numParams = 1;
	char *functionName = "EnterCriticalSection";
	char *categoryName = "DANGEROUS";
	
	void *params[numParams] = { &lpCriticalSection };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realEnterCriticalSection == NULL)
		realEnterCriticalSection = (EnterCriticalSectionFunction)library->GetOriginalFunction(functionName);
		
	if (realEnterCriticalSection != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, EnterCriticalSectionCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "VOID", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		lstrcpyWReplacement
// Description: See MSDN lstrcpyW function
// Parameters:	See MSDN lstrcpyW parameters
// Return Value: See MSDN lstrcpyW return value
//*************************************************************************
LPWSTR WINAPI lstrcpyWReplacement(LPWSTR lpString1, LPWSTR lpString2)
{
	const int numParams = 2;
	char *functionName = "lstrcpyW";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0] };
	ParameterType paramTypes[numParams] = { OutWideStringType, WideStringType };
	
	if (reallstrcpyW == NULL)
		reallstrcpyW = (lstrcpyWFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcpyW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPWSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcpyWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPWSTR", &errorCode, 0, true))
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
// Method:		lstrcatWReplacement
// Description: See MSDN lstrcatW function
// Parameters:	See MSDN lstrcatW parameters
// Return Value: See MSDN lstrcatW return value
//*************************************************************************
LPWSTR WINAPI lstrcatWReplacement(LPWSTR lpString1, LPWSTR lpString2)
{
	const int numParams = 2;
	char *functionName = "lstrcatW";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0] };
	ParameterType paramTypes[numParams] = { OutWideStringType, WideStringType };
	
	if (reallstrcatW == NULL)
		reallstrcatW = (lstrcatWFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcatW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPWSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcatWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPWSTR", &errorCode, 0, true))
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
// Method:		lstrcpynWReplacement
// Description: See MSDN lstrcpynW function
// Parameters:	See MSDN lstrcpynW parameters
// Return Value: See MSDN lstrcpynW return value
//*************************************************************************
LPWSTR WINAPI lstrcpynWReplacement(LPWSTR lpString1, LPCWSTR lpString2, int iMaxLength)
{
	const int numParams = 3;
	char *functionName = "lstrcpynW";
	char *categoryName = "DANGEROUS";
	SiString str[] = { (wchar_t *)lpString2 };
	void *params[numParams] = { &lpString1, &str[0], &iMaxLength };
	ParameterType paramTypes[numParams] = { PointerType, WideStringType, IntegerType };
	
	if (reallstrcpynW == NULL)
		reallstrcpynW = (lstrcpynWFunction)library->GetOriginalFunction(functionName);
		
	if (reallstrcpynW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		LPWSTR returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, lstrcpynWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "LPWSTR", &errorCode, 0, true))
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
// Method:		LeaveCriticalSectionReplacement
// Description: See MSDN LeaveCriticalSection function
// Parameters:	See MSDN LeaveCriticalSection parameters
// Return Value: See MSDN LeaveCriticalSection return value
//*************************************************************************
void WINAPI LeaveCriticalSectionReplacement(LPCRITICAL_SECTION lpCriticalSection)
{
	const int numParams = 1;
	char *functionName = "LeaveCriticalSection";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpCriticalSection };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realLeaveCriticalSection == NULL)
		realLeaveCriticalSection = (LeaveCriticalSectionFunction)library->GetOriginalFunction(functionName);
		
	if (realLeaveCriticalSection != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, LeaveCriticalSectionCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "void", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		DeleteCriticalSectionReplacement
// Description: See MSDN DeleteCriticalSection function
// Parameters:	See MSDN DeleteCriticalSection parameters
// Return Value: See MSDN DeleteCriticalSection return value
//*************************************************************************
void WINAPI DeleteCriticalSectionReplacement(LPCRITICAL_SECTION lpCriticalSection)
{
	const int numParams = 1;
	char *functionName = "DeleteCriticalSection";
	char *categoryName = "PROCESS";
	
	void *params[numParams] = { &lpCriticalSection };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realDeleteCriticalSection == NULL)
		realDeleteCriticalSection = (DeleteCriticalSectionFunction)library->GetOriginalFunction(functionName);
		
	if (realDeleteCriticalSection != NULL)
	{
		DWORD errorCode, returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, DeleteCriticalSectionCaller, categoryName, functionName, numParams, params, paramTypes, &returnValue, "void", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		SetLastError(errorCode);
	}
	
	
}
//*************************************************************************
// Method:		CreateIoCompletionPortReplacement
// Description: See MSDN CreateIoCompletionPort function
// Parameters:	See MSDN CreateIoCompletionPort parameters
// Return Value: See MSDN CreateIoCompletionPort return value
//*************************************************************************
HANDLE WINAPI CreateIoCompletionPortReplacement(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads)
{
	const int numParams = 4;
	char *functionName = "CreateIoCompletionPort";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &FileHandle, &ExistingCompletionPort, &CompletionKey, &NumberOfConcurrentThreads };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realCreateIoCompletionPort == NULL)
		realCreateIoCompletionPort = (CreateIoCompletionPortFunction)library->GetOriginalFunction(functionName);
		
	if (realCreateIoCompletionPort != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, CreateIoCompletionPortCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
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
// Method:		GetQueuedCompletionStatusReplacement
// Description: See MSDN GetQueuedCompletionStatus function
// Parameters:	See MSDN GetQueuedCompletionStatus parameters
// Return Value: See MSDN GetQueuedCompletionStatus return value
//*************************************************************************
BOOL WINAPI GetQueuedCompletionStatusReplacement(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED * lpOverlapped, DWORD dwMilliseconds)
{
	const int numParams = 5;
	char *functionName = "GetQueuedCompletionStatus";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &CompletionPort, &lpNumberOfBytes, &lpCompletionKey, &lpOverlapped, &dwMilliseconds };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realGetQueuedCompletionStatus == NULL)
		realGetQueuedCompletionStatus = (GetQueuedCompletionStatusFunction)library->GetOriginalFunction(functionName);
		
	if (realGetQueuedCompletionStatus != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, GetQueuedCompletionStatusCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
// Method:		PostQueuedCompletionStatusReplacement
// Description: See MSDN PostQueuedCompletionStatus function
// Parameters:	See MSDN PostQueuedCompletionStatus parameters
// Return Value: See MSDN PostQueuedCompletionStatus return value
//*************************************************************************
BOOL WINAPI PostQueuedCompletionStatusReplacement(HANDLE CompletionPort, DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOverlapped)
{
	const int numParams = 4;
	char *functionName = "PostQueuedCompletionStatus";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &CompletionPort, &dwNumberOfBytesTransferred, &dwCompletionKey, &lpOverlapped };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realPostQueuedCompletionStatus == NULL)
		realPostQueuedCompletionStatus = (PostQueuedCompletionStatusFunction)library->GetOriginalFunction(functionName);
		
	if (realPostQueuedCompletionStatus != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		BOOL returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, PostQueuedCompletionStatusCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "BOOL", &errorCode, 0, true))
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
