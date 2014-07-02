//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Debugger.cpp
//
// DESCRIPTION: Contains implementation of the Holodeck debugger.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 FEB 2004		 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "Debugger.h"
#include "HoloScriptApplication.h"
#include "StringConverter.h"

#undef CreateDirectory // so we can use .NET version

using namespace HoloScript;

typedef BOOL (WINAPI *DebugSetProcessKillOnExitFunction)(BOOL);
typedef BOOL (WINAPI *DebugActiveProcessStopFunction)(DWORD);


//*************************************************************************
// Method:		HoloScriptDebugger
// Description: Constructor for HoloScriptDebugger class
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
HoloScriptDebugger::HoloScriptDebugger()
{
	attachedApp = NULL;
	terminateDebugThread = false;

	RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
	String* installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
	String* dbgHelpPath = String::Concat(installPath, "\\dbghelp.dll");

	SiString dbgHelpPathString;
	StringConverter::GetInstance()->ConvertToSiString(dbgHelpPath, &dbgHelpPathString);

	HMODULE hDbgHelpDll = LoadLibraryW((wchar_t*)dbgHelpPathString);
	if (hDbgHelpDll)
	{
		MiniDumpWriteDumpFunc = (BOOL (__stdcall *)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
			PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION))GetProcAddress(hDbgHelpDll, "MiniDumpWriteDump");
	}
}


//*************************************************************************
// Method:		~HoloScriptDebugger
// Description: Destructor for HoloScriptDebugger class
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
HoloScriptDebugger::~HoloScriptDebugger()
{
	if (attachedApp)
		Detach();
	if (hDbgHelpDll)
		FreeLibrary(hDbgHelpDll);
}


//*************************************************************************
// Method:		Attach
// Description: Attaches the debugger to an application
//
// Parameters:
//	app - Application to attach to
//
// Return Value: true on success
//*************************************************************************
bool HoloScriptDebugger::Attach(HoloScriptApplication* app)
{
	if (!app->GetApplicationProcessID(processId))
		return false;

	ThreadStart* start = new ThreadStart(this, DebugThreadProc);
	debugThread = new Thread(start);
	debugThread->IsBackground = true;
	debugThread->Start();

	attachedApp = app;
	return true;
}


//*************************************************************************
// Method:		Detach
// Description: Detaches the debugger from the current application
//
// Parameters:
//	None
//
// Return Value: true on success
//*************************************************************************
bool HoloScriptDebugger::Detach()
{
	if (!attachedApp)
		return true;

	// Force debug thread to stop, and wait for this to happen
	terminateDebugThread = true;
	debugThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
	terminateDebugThread = false;

	debugThread = NULL;
	attachedApp = NULL;
	return true;
}


//*************************************************************************
// Method:		DebugThreadProc
// Description: Thread procedure that contains the debugging loop
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void HoloScriptDebugger::DebugThreadProc()
{
	DEBUG_EVENT debugEvent;

	// Attach to the desired process
	if (!DebugActiveProcess(processId))
		return;

	HMODULE kernel32Module = GetModuleHandle("kernel32.dll");
	DebugSetProcessKillOnExitFunction killOnExitFunc = NULL;
	
	if (kernel32Module)
	{
		killOnExitFunc = (DebugSetProcessKillOnExitFunction)GetProcAddress(kernel32Module,
			"DebugSetProcessKillOnExit");
	}

	if (killOnExitFunc)
	{
		// Don't want to terminate app when done
		killOnExitFunc(FALSE);
	}

	// Start the debugger loop
	while (!terminateDebugThread)
	{
		if (!WaitForDebugEvent(&debugEvent, 200))
			continue;

		if (debugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			if (debugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
			{
				// Continue from breakpoint exception (one happens on both process attach and exit)
				ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
			}
			else
			{
				// Pass exception info to the user
				ExceptionInfo info;

				SYSTEMTIME time;
				GetLocalTime(&time);
				char buffer[64];
				wsprintf(buffer, "%2d/%2d/%2d  %2d:%2d:%2d:%3d", time.wMonth, time.wDay, time.wYear, time.wHour,
					time.wMinute, time.wSecond, time.wMilliseconds);
				info.timeStamp = new String(buffer);

				info.firstChance = (debugEvent.u.Exception.dwFirstChance != 0) ? true : false;

				if ((!info.firstChance) || (logFirstChance))
				{
					info.threadId = debugEvent.dwThreadId;
					info.exceptionCode = debugEvent.u.Exception.ExceptionRecord.ExceptionCode;
					info.exceptionAddress = (unsigned int)(size_t)debugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
					info.miniDumpFile = ""; // filled in later if a valid minidump could be created

					if (info.exceptionCode == EXCEPTION_ACCESS_VIOLATION)
					{
						// Only access violation has writeAccess and memoryAddress parameters
						info.writeAccess = (debugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0] != 0) ? true : false;
						info.memoryAddress = debugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1];
					}

					int index = 1;
					String* fileName;
					String* exceptionFolder = String::Concat(attachedApp->ProjectFolder,
						attachedApp->ProjectTitle, S"_Exceptions");
					while (true)
					{
						fileName = String::Concat(exceptionFolder, S"\\Exception", __box(index)->ToString(), S".dmp");
						if (!File::Exists(fileName))
						{
							// File doesn't exist yet
							break;
						}
						index++;
					}

					if (!Directory::Exists(exceptionFolder))
						Directory::CreateDirectory(exceptionFolder);

					SiString fileNameString;
					Holodeck::StringConverter::GetInstance()->ConvertToSiString(fileName, &fileNameString);
					if (GenerateMinidump(debugEvent.dwThreadId, &debugEvent.u.Exception.ExceptionRecord, (wchar_t*)fileNameString))
						info.miniDumpFile = fileName;

					if (OnExceptionThrown)
						OnExceptionThrown->Invoke(processId, info);
				}
			}
		}
		else if (debugEvent.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
		{
			// Debug APIs require debugger loop to close file handle
			CloseHandle(debugEvent.u.CreateProcessInfo.hFile);
		}
		else if (debugEvent.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
		{
			// Debug APIs require debugger loop to close file handle
			CloseHandle(debugEvent.u.LoadDll.hFile);
		}
		else if (debugEvent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
			break;
		ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
	}

	DebugActiveProcessStopFunction debugStopFunc = NULL;
	if (kernel32Module)
	{
		debugStopFunc = (DebugActiveProcessStopFunction)GetProcAddress(kernel32Module,
			"DebugActiveProcessStop");
	}

	if (debugStopFunc)
	{
		// Detach the debugger
		debugStopFunc(processId);
	}
	else
	{
		// Detach not supported, just kill the app
		HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
		if (process)
			TerminateProcess(process, 0);
	}
}


//*************************************************************************
// Method:		GenerateMinidump
// Description: Generates a ".dmp" file for a crash
//
// Parameters:
//	threadId - ID of thread that caused the exception
//  exceptionRecord - Exception record from the debug event
//  fileName - File name of the ".dmp" file to write
//
// Return Value: true on success
//*************************************************************************
bool HoloScriptDebugger::GenerateMinidump(DWORD threadId, EXCEPTION_RECORD* exceptionRecord, wchar_t* fileName)
{
	bool success = false;

	if (MiniDumpWriteDumpFunc)
	{
		HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			// Prepare the exception information for the minidump
			MINIDUMP_EXCEPTION_INFORMATION exceptInfo;
			EXCEPTION_POINTERS exceptPtrs;
			CONTEXT context;

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
			if (hProcess != NULL)
			{
				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
				if (hThread != NULL)
				{
					context.ContextFlags = CONTEXT_FULL;
					GetThreadContext(hThread, &context);

					CloseHandle(hThread);

					exceptPtrs.ContextRecord = &context;
					exceptPtrs.ExceptionRecord = exceptionRecord;

					exceptInfo.ThreadId = threadId;
					exceptInfo.ExceptionPointers = &exceptPtrs;
					exceptInfo.ClientPointers = FALSE; // info is in our process space

					// Write the minidump to the new file
					success = true;

					// Generate full dump
					if (!MiniDumpWriteDumpFunc(hProcess, processId, hFile, MiniDumpWithFullMemory,
						&exceptInfo, NULL, NULL))
					{
						// Failed to write minidump
						success = false;
					}

					// Close the minidump file
					CloseHandle(hFile);

					if (!success)
						DeleteFileW(fileName);
					else
					{
						// Compress the dump to save space
						hFile = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
						if (hFile != INVALID_HANDLE_VALUE)
						{
							USHORT compressionFormat = COMPRESSION_FORMAT_DEFAULT;
							DWORD dwBytes;
							DeviceIoControl(hFile, FSCTL_SET_COMPRESSION, &compressionFormat, sizeof(USHORT), NULL, 0, &dwBytes, NULL);
							CloseHandle(hFile);
						}
					}
				}
				CloseHandle(hProcess);
			}
		}
	}

	return success;
}
