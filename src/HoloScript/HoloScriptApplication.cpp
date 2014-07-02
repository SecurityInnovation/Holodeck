//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HoloScriptApplication.cpp
//
// DESCRIPTION: Contains implementation of the class HoloScriptApplication.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 DEC 2002		 B. Shirey	 File created.
// 04 FEB 2003       J. Taylor   Fixed a bug in which CreateScheduledTest was no returning testID
// 05 Feb 2003       J. Taylor   Fixed a bug in which widestring params were not getting copied properly in CreateScheduledTest
// 06 Feb 2003       J. Taylor   Fixed some TestID string handling problems in CreateScheduledTest
// 11 Feb 2003       A. Kakrania Fixed GetMonitorInformation function to return values correctly
// 15 Feb 2003       A. Kakrania Added functionlity to search within the log file (SearchInLogFile)
//*************************************************************************
#include <stdio.h>
#include <vcclr.h>
#include <wchar.h>
#include <windows.h>
#include <psapi.h>
#include "HoloScriptRegistry.h"
#include "HoloScriptApplication.h"
#include "Debugger.h"
#include "ServiceHelper.h"
#include "StringConverter.h"

#undef GetTempFileName
#undef GetCurrentDirectory
#undef SetCurrentDirectory

namespace HoloScript
{
	//*************************************************************************
	// Method:		HoloScriptApplication
	// Description: Constructor for the HoloScriptApplication class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoloScriptApplication::HoloScriptApplication() : settingsMgr(NULL), logReceiver(NULL),
		appNeedsToBeTerminated(true), pendingServiceToStart(NULL), shouldRestart(true)
	{
		heat = new CHeatApp();

		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		
		String *installPath, *functionFolder;
		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!db->IsLoaded)
			db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		interceptionList = new ArrayList();

		if (waitForLaunchedServiceEvent == NULL)
		{
			launchedServiceProcessId = 0;
			waitForLaunchedServiceEvent = new Event();
			waitForLaunchedServiceEvent->Create();
		}

		if (readyToContinueLaunchEvent == NULL)
		{
			readyToContinueLaunchEvent = new Event();
			readyToContinueLaunchEvent->Create();
		}

		if (waitForServiceStopEvent == NULL)
		{
			waitForServiceStopEvent = new Event();
			waitForServiceStopEvent->Create();
		}

		logReader = new LogReader();

		scheduledTestWorkQueue = new ArrayList();

		this->processSymbolsArray = NULL;
		this->triedGettingSymbols = false;
		this->firstResume = true;
	}

	//*************************************************************************
	// Method:		~HoloScriptApplication
	// Description: Destructor for the HoloScriptApplication class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoloScriptApplication::~HoloScriptApplication()
	{
		CleanUp();

		//don't cleanup heat, settingsmanager, or logreader until destructor is called
		if (heat != NULL)
		{
			delete heat;
			heat = NULL;
		}

		if (settingsMgr != NULL)
		{
			delete settingsMgr;
			settingsMgr = NULL;
		}

		try
		{
			if (logReader && !savedLog)
				File::Delete(logReader->LogFileName);
		}
		catch(...)
		{
		}

		if (logReader != NULL)
		{
			delete logReader;
			logReader = NULL;
		}
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Performs common cleanup tasks
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::CleanUp()
	{
		DetachDebuggerFromProcess(); // detach any active debugger

		if (isService)
		{
			ResumeApplication();

			waitForServiceStopEvent->Release();

			ThreadStart *stopThreadStart = new ThreadStart(this, StopServiceThreadFunction);
			Thread *stopThread = new Thread(stopThreadStart);
			stopThread->Start();

			waitForServiceStopEvent->Acquire(WAIT_FOR_SERVICE_TIMEOUT);
		}

		if (heat != NULL)
		{
			if (TerminateOnClose)
				heat->terminateApp();
			else
				heat->detachFromApp(); 
		}

		if (logReceiver != NULL)
		{
			logReceiver->RemoveLogReceivedListener(logReader);
			delete logReceiver;
			logReceiver = NULL;
		}

		if (childProcListener != NULL)
		{
			delete childProcListener;
			childProcListener = NULL;
		}

		if (threadRunning)
		{
			threadRunning = false;
			cantCreateLogFileEvent->Signal();
			cantCreateLogFileListeningThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
			delete cantCreateLogFileEvent;
			cantCreateLogFileEvent = NULL;
		}

		if (scheduledTestWorkerRunning)
		{
			scheduledTestWorkerRunning = false;
			scheduledTestQueueEvent->Signal();
			scheduledTestWorkerThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
			delete scheduledTestQueueEvent;
			delete scheduledTestQueueCompleteEvent;
			scheduledTestQueueEvent = NULL;
			scheduledTestQueueCompleteEvent = NULL;
		}

		if (testPauseWatchThreadRunning)
		{
			testPauseWatchThreadRunning = false;
			testPauseWatchThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
			testPauseWatchThread = NULL;
		}

		StopProcessWatching();

		try
		{
			if (logReader && !savedLog)
				File::Delete(logReader->LogFileName);
		}
		catch(...)
		{
		}

	}

	//*************************************************************************
	// Method:		get_ApplicationName
	// Description: sets the current application name
	//
	// Parameters:
	//	None
	//
	// Return Value: the current application name
	//*************************************************************************
	String *HoloScriptApplication::get_ApplicationName()
	{
		if (applicationToLaunch == NULL)
			return "Unknown";
		int index = applicationToLaunch->LastIndexOf("\\");
		if (index != -1)
		{
			index++;
			String *appName = applicationToLaunch->Substring(index, applicationToLaunch->Length-index);
			return appName;
		}
		return applicationToLaunch;
	}

	//*************************************************************************
	// Method:		get_IsRunning
	// Description: gets whether or not the current application is running
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the application is running, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::get_IsRunning() 
	{ 
		unsigned int procID;

		try
		{
			if (process == NULL)
			{
				GetApplicationProcessID (procID);
				process = Process::GetProcessById(procID);
			}
			else
				process->Refresh();

			if ((process == NULL) || (process->HasExited))
				return false;
			else
				return true;
		}
		catch(...)
		{
			//can't get processbyid because it isn't running
			return false;
		}
	}

	//*************************************************************************
	// Method:		RemoveOnLogGeneratedHandler
	// Description: removes a log generated delegate handler from the log reader
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::RemoveOnLogGeneratedHandler(LogsGeneratedDelegate *value)
	{
		if (!value)
			return;

		if (logReader)
			logReader->OnLogGenerated -= value;
	}

	//*************************************************************************
	// Method:		AddOnLogGeneratedHandler
	// Description: removes a log generated delegate handler from the log reader
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::AddOnLogGeneratedHandler(LogsGeneratedDelegate *value)
	{
		if (!value)
			return;

		if (logReader)
			logReader->OnLogGenerated += value;
	}

	//*************************************************************************
	// Method:		GetLogEntryFromFile
	// Description: Get a given log entry from the log file
	//
	// Parameters:
	//	info - The info of the log block to get it from
	//	index - The log entry number in the info block to get
	//  filePosition - The position of the file of the entry
	//
	// Return Value: The log entry at the given location
	//*************************************************************************
	LogItem *HoloScriptApplication::GetLogEntryFromFile(ReceivedLogInfo *info, int index, unsigned long &filePosition)
	{
		if (logReader)
			return logReader->GetLogEntryFromFile(info, index, filePosition);

		return NULL;
	}

	//*************************************************************************
	// Method:		GetLogEntryAtFilePosition
	// Description: Get a given log entry from the aut at a specified location
	//	in the file
	//
	// Parameters:
	//	filePos - The file position of the log entry to get
	//
	// Return Value: The log entry at the location given
	//*************************************************************************
	LogItem *HoloScriptApplication::GetLogEntryAtFilePosition(unsigned long filePos)
	{
		if (logReader && (filePos != 0))
			return logReader->GetLogEntryAtFilePosition(filePos);

		return NULL;
	}

	//*************************************************************************
	// Method:		GetLogEntryAt
	// Description: Get a given log entry from the aut
	//
	// Parameters:
	//	index - The log entry number to get
	//  filePosition - The position of the entry in the file
	//
	// Return Value: The log entry at the index given
	//*************************************************************************
	LogItem *HoloScriptApplication::GetLogEntryAt(int index, unsigned long &filePosition)
	{
		if (logReader)
			return logReader->GetLogEntryAt(index, filePosition);
		return NULL;
	}

	//*************************************************************************
	// Method:		ExportToDiffFile
	// Description: Exports all log entries to a diff file in plain text format
	//
	// Parameters:
	//	diffFileName - the filename to store the logs in
	//	filePositionsToLog - A list of file positions to export.  If NULL the
	//		entire log file will be exported
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::ExportToDiffFile(String *diffFileName, ArrayList *filePositionsToLog)
	{
		if (logReader)
			logReader->ExportToDiffFile(diffFileName, filePositionsToLog);
	}

	//*************************************************************************
	// Method:		OpenLogFile
	// Description: sets the file name to use and re-opens the file
	//
	// Parameters:
	//	newLogFileName - the filename to read
	//
	// Return Value: true on success, false on failure
	//*************************************************************************
	bool HoloScriptApplication::OpenLogFile(String *newLogFileName)
	{
		if (heat && heat->isReady())
		{
			// Already running an application.  We can't change which log file the application is going
			// to use at run time, so return an error.
			return false;
		}

		String *logFileName = newLogFileName;
		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);
		HANDLE logFile = CreateFileW(fullLogFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

		if (logFile == INVALID_HANDLE_VALUE)
			return false;
		
		DWORD numRead = 0;

		// get the version info
		unsigned long versionSize;
		ReadFile(logFile, &versionSize, sizeof(unsigned long), &numRead, NULL);

		unsigned char *version = new unsigned char[versionSize + 1];
		ReadFile(logFile, version, versionSize, &numRead, NULL);
		version[versionSize] = '\0';

		// v2_1 didn't have a version field
		if (strstr((char *)version, "HDV") == NULL)
			logReader->Version = HolodeckVersion::V2_1;
		else if (strcmp((char *)version, "HDV2_5") == 0)
			logReader->Version = HolodeckVersion::V2_5;

		delete[] version;
		CloseHandle(logFile);

		bool retVal = logReader->OpenLogFile(newLogFileName);
		if (retVal)
			savedLog = true;

		return retVal;
	}

	//*************************************************************************
	// Method:		DeleteSavedLogFile
	// Description: Deletes a saved log file from disk
	//
	// Parameters:
	//	newLogFileName - the saved filename to delete
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::DeleteSavedLogFile(String *logFileName)
	{
		if (logReader != NULL)
		{
			delete logReader;
			logReader = NULL;
		}

		if (File::Exists (logFileName))
		{
			try
			{
				File::Delete (logFileName);
			}

			catch (Exception *)
			{
				return false;
			}
		}

		return true;
	}

	//*************************************************************************
	// Method:		SearchInLogFile
	// Description: Searches the log file for a specified string
	//
	// Parameters:
	//	searchString - The string to search for
	//  startRecord  - The record# in the log file to start searching from
	//
	// Return Value: the record# of the entry matching the query, -1 if not found
	//*************************************************************************
	int HoloScriptApplication::SearchInLogFile(String * searchString, int startRecord)
	{
		searchString = searchString->ToUpper();
		if (!logReader)
			return -1;

		for (int i=startRecord; i < (int)logReader->LogEntryCount; i++)
		{
			unsigned long filePos;
			LogItem *item = GetLogEntryAt (i, filePos);

			if (item != NULL)
			{
				if ((item->Category->ToUpper()->IndexOf (searchString)) > -1)
					return i;

				if ((item->DllName->ToUpper()->IndexOf (searchString)) > -1)
					return i;

				if ((item->ErrorCode->ToUpper()->IndexOf (searchString)) > -1)
					return i;

				if ((item->Function->ToUpper()->IndexOf (searchString)) > -1)
					return i;

				if ((item->ReturnValue->ToUpper()->IndexOf (searchString)) > -1)
					return i;

				for (int paramCount = 0; paramCount < item->Parameters->Count; paramCount++)
				{
					String *value = NULL;
					if (item->Parameters->Item[paramCount] != NULL)
						value = item->Parameters->Item[paramCount]->ToString();

					if ((value != NULL) && (value->ToUpper()->IndexOf (searchString) > -1))
						return i;
				}
			}

			if (item != NULL)
				delete item;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		InterceptFunctionsFromDatabase
	// Description: attaches replacement libraries to the intercepted application
	//	by reading the data from the function database
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::InterceptFunctionsFromDatabase()
	{
		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		
		String *installPath, *functionFolder, *pluginFolder;
		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		pluginFolder = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));

		String *pluginPath = String::Concat(installPath, "\\", pluginFolder);
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		Array *functions = db->Functions;

		if (functions == NULL)
			return false;

		IEnumerator *enumerator = functions->GetEnumerator();
		const int stringSize = 511;
		char name[stringSize + 1], originalDll[stringSize + 1];
		char replacementName[stringSize + 1], interceptedDll[stringSize + 1];
		const __wchar_t __pin *wideName = NULL;
		const __wchar_t __pin *wideDll = NULL;
		const __wchar_t __pin *wideReplacementName = NULL;
		const __wchar_t __pin *wideInterceptedDll = NULL;
		String *fullLibraryPath = NULL;

		// loop through the functions found in the database
		while (enumerator->MoveNext())
		{
			InterceptedFunction *function = __try_cast<InterceptedFunction *>(enumerator->Current);

			fullLibraryPath = String::Concat(pluginPath, "\\", function->InterceptedDll);

			wideName = PtrToStringChars(function->Name);
			wideDll = PtrToStringChars(function->OriginalDll);
			wideReplacementName = PtrToStringChars(function->ReplacementFunction);
			wideInterceptedDll = PtrToStringChars(fullLibraryPath);

			wcstombs(name, wideName, stringSize);
			wcstombs(originalDll, wideDll, stringSize);
			wcstombs(replacementName, wideReplacementName, stringSize);
			wcstombs(interceptedDll, wideInterceptedDll, stringSize);

			DWORD retVal = heat->interceptFunc(name, originalDll, replacementName, interceptedDll);
		}

		return true;
	}

	//*************************************************************************
	// Method:		InterceptFunctionsFromInterceptionList
	// Description: attaches replacement libraries to the intercepted application
	//	by reading the data from InterceptionList
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::InterceptFunctionsFromInterceptionList()
	{

		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		
		String *installPath, *functionFolder, *pluginFolder;
		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		pluginFolder = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));

		String *pluginPath = String::Concat(installPath, "\\", pluginFolder);
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		const int stringSize = 511;
		char name[stringSize + 1], originalDll[stringSize + 1];
		char replacementName[stringSize + 1], interceptedDll[stringSize + 1];
		const __wchar_t __pin *wideName = NULL;
		const __wchar_t __pin *wideDll = NULL;
		const __wchar_t __pin *wideReplacementName = NULL;
		const __wchar_t __pin *wideInterceptedDll = NULL;
		String *fullLibraryPath = NULL;

		// loop through the functions found in the interception list
		for (int i=0; i< interceptionList->Count; i++)
		{
			InterceptionInfo * iFunc = dynamic_cast <InterceptionInfo *> (interceptionList->Item[i]);
			ArrayList *functions = db->GetAllFunctionsByName(iFunc->functionName);
			if (!functions)
				continue;

			IEnumerator *funcEnumerator = functions->GetEnumerator();
			while (funcEnumerator->MoveNext())
			{
				InterceptedFunction *function = __try_cast<InterceptedFunction *>(funcEnumerator->Current);
				if (!function)
					continue;

				fullLibraryPath = iFunc->redirectedDllName;
				wideName = PtrToStringChars(function->Name);
				wideDll = PtrToStringChars(function->OriginalDll);
				wideReplacementName = PtrToStringChars(iFunc->redirectedFunctionName);
				wideInterceptedDll = PtrToStringChars(fullLibraryPath);

				wcstombs(name, wideName, stringSize);
				wcstombs(originalDll, wideDll, stringSize);
				wcstombs(replacementName, wideReplacementName, stringSize);
				wcstombs(interceptedDll, wideInterceptedDll, stringSize);

				DWORD retVal = heat->interceptFunc(name, originalDll, replacementName, interceptedDll);
			}
		}

		return true;
	}

	//*************************************************************************
	// Method:		CantCreateLogFileThreadFunction
	// Description: thread function to wait for notice that the aut can't create
	//	its log file
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::CantCreateLogFileThreadFunction()
	{
		while (threadRunning)
		{
			cantCreateLogFileEvent->Acquire();
			if (threadRunning && OnCantCreateLogFile)
				OnCantCreateLogFile->Invoke();
		}
	}

	//*************************************************************************
	// Method:		IsSystemProcess
	// Description: Determines whether or not a process is an important system
	//	process (one that should never be attached to)
	//
	// Parameters:
	//	processIDToCheck - the id of the running process to check
	//
	// Return Value: true if it is a system process, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::IsSystemProcess(unsigned long processIDToCheck)
	{
		System::Diagnostics::Process *currentProcess;
		try
		{
			currentProcess = System::Diagnostics::Process::GetProcessById(processIDToCheck);
		}
		catch (...)
		{
			return false;
		}
		if (!currentProcess)
			return false;
		
		String *procName;
		try
		{
			procName = currentProcess->MainModule->FileName;
			int slashPosition = procName->LastIndexOf('\\');
			if (slashPosition != -1)
				procName = procName->Substring(slashPosition + 1);
		}
		catch(...)
		{
			procName = currentProcess->ProcessName;
		}

		procName = procName->ToLower()->Trim();
		if ((processIDToCheck == 0) ||
			(processIDToCheck == 4) ||
			(processIDToCheck == 8) ||
			(procName->Equals("holodeckgui.exe")) ||
			(procName->Equals("services.exe")) ||
			(procName->Equals("smss.exe")) ||
			(procName->Equals("csrss.exe")) ||
			(procName->Equals("winlogon.exe")) ||
			(procName->Equals("svchost.exe")) ||
			(procName->Equals("spoolsv.exe")) ||
			(procName->Equals("regsvc.exe")) ||
			(procName->Equals("wmiprvse.exe")) ||
			(procName->Equals("lsass.exe")))
			return true;

		return false;
	}

	//*************************************************************************
	// Method:		IsSystemProcess
	// Description: Determines whether or not a process is an important system
	//	process (one that should never be attached to)
	//
	// Parameters:
	//	processToCheck - the name of the process to check
	//
	// Return Value: true if it is a system process, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::IsSystemProcess(String *processToCheck)
	{
		String *procName = processToCheck;
		procName = procName->ToLower()->Trim();
		int pos = procName->LastIndexOf("\\");
		if (pos != -1)
			procName = procName->Substring(pos + 1);

		if ((procName->Equals("system")) ||
			(procName->Equals("system idle process")) ||
			(procName->Equals("holodeckgui.exe")) ||
			(procName->Equals("services.exe")) ||
			(procName->Equals("smss.exe")) ||
			(procName->Equals("csrss.exe")) ||
			(procName->Equals("winlogon.exe")) ||
			(procName->Equals("svchost.exe")) ||
			(procName->Equals("spoolsv.exe")) ||
			(procName->Equals("regsvc.exe")) ||
			(procName->Equals("wmiprvse.exe")) ||
			(procName->Equals("lsass.exe")))
			return true;

		return false;
	}

	//*************************************************************************
	// Method:		IsSystemService
	// Description: Determines whether or not a process is an important system
	//	service (one that should never be attached to)
	//
	// Parameters:
	//	serviceNameToCheck - the name of the service to check
	//	canAttachTo - also check services we can't attach to?
	//
	// Return Value: true if it is a system service, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::IsSystemService(String *serviceNameToCheck, bool canAttachTo)
	{
		String *name = serviceNameToCheck->ToLower()->Trim();
		if ((name->Equals("nt lm security support provider")) ||
			(name->Equals("ntlmssp")) ||
			(name->Equals("plug and play")) ||
			(name->Equals("plugplay")) ||
			(name->Equals("print spooler")) ||
			(name->Equals("spooler")) ||
			(name->Equals("remote procedure call (rpc)")) ||
			(name->Equals("rpcss")) ||
			(name->Equals("windows management instrumentation")) ||
			(name->Equals("winmgmt")) ||
			(name->Equals("windows management instrumentation driver extensions")) ||
			(name->Equals("wmi")) ||
			(name->Equals("windows management instrumentation provider service")) ||
			(name->Equals("wmiprvse")) ||
			(name->Equals("event log")) ||
			(name->Equals("eventlog")) ||
			(name->Equals("ipv6 helper service")) ||
			(name->Equals("6to4")) ||
			(name->Equals("net logon")) ||
			(name->Equals("netlogon")) ||
			(name->Equals("ipsec services")) ||
			(name->Equals("policyagent")) ||
			(name->Equals("protected storage")) ||
			(name->Equals("protectedstorage")) ||
			(name->Equals("security accounts manager")) ||
			(name->Equals("samss")))
			return true;

		// now see if the image path is a known process we shouldn't attach to but isn't in the list above
		ServiceController *service = new ServiceController(name);
		if (!service)
			return false;

		RegistryKey *serviceKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(String::Concat(SERVICES_BASE_KEY_NAME, "\\", service->ServiceName));
		if (!serviceKey)
			return false;

		Object *pathObj = serviceKey->GetValue(IMAGE_PATH_VALUE_NAME);
		if (pathObj)
		{
			String *path = pathObj->ToString()->Trim()->ToLower();
			if ((path->StartsWith("lsass.exe")) ||
				(path->StartsWith("services.exe")) ||
				(path->IndexOf("\\lsass.exe") != -1) ||
				(path->IndexOf("\\services.exe") != -1))
			{
				return true;
			}
		}

		// now check for svchost.exe processes that can't be attached to
		if (canAttachTo)
		{
			ServiceHelper *helper = new ServiceHelper();
			String *group = helper->GetCurrentSharedServiceGroup(name);
			if (group)
			{
				if (group->Trim()->ToLower()->Equals("netsvcs"))
					return true;
			}
		}

		serviceKey->Close();
		service->Close();		

		return false;
	}

	//*************************************************************************
	// Method:		AttachApplication
	// Description: Attaches HoloScript to a running process
	//
	// Parameters:
	//	processID - the id of the running process to attach to
	//	pauseOnAttach - true if you want to pause the application on attachment,
	//		false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::AttachApplication(unsigned long processID, bool pauseOnAttach, ArrayList *filteredFunctionList, bool suspendedApp)
	{
		return AttachApplication (processID, pauseOnAttach, filteredFunctionList, NULL, false, suspendedApp, false);
	}

	//*************************************************************************
	// Method:		AttachApplication
	// Description: Attaches HoloScript to a running process
	//
	// Parameters:
	//	processID - the id of the running process to attach to
	//	pauseOnAttach - true if you want to pause the application on attachment,
	//		false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//	exporterLog	- the path and name of the file to use for logging (if NULL, default is used)
	//	UseExternalInterceptionList - used to signify that the LibraryExporter is using the function
	//				and interception of functions should be performed from a different interception list
	//	injectOnlyServiceExeLibrary - true to inject only serviceexereplacementlibrary, false
	//		to inject everything else
	//
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::AttachApplication(unsigned long processID, bool pauseOnAttach, ArrayList *filteredFunctionList, String * exporterLog, bool UseExternalInterceptionList, bool suspendedApp, bool injectOnlyServiceExeLibrary)
	{
		//If filteredFunctionList is empty then create an emply list
		if (filteredFunctionList == NULL)
			filteredFunctionList = new ArrayList();

		//get application full path
		try
		{
			bool nameSet = SetApplicationName(processID);
			if (!nameSet)
			{
				if (isService && (serviceName != NULL))
					applicationToLaunch = ServiceHelper::GetServiceExecutablePath(serviceName);
				else
					applicationToLaunch = "Unknown";
			}
		}
		catch(...)
		{
			throw new Exception("The process you selected is no longer running");
		}

		if (!isService && !injectOnlyServiceExeLibrary && IsSystemProcess(processID))
		{
			throw new Exception("This is an important system process that should not be attached to as it could make the system unstable.");
		}
		else if (isService && IsSystemService(serviceName, !appAttachedTo))
		{
			throw new Exception("This is an important system service that should not be attached to as it could make the system unstable.");
		}

		appAttachedTo = true;
		isService = false;
		serviceName = NULL;

		HANDLE token;
		TOKEN_PRIVILEGES privileges;

		privileges.PrivilegeCount = 1;

		// get the process's access token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token))
			return false;

		// get the luid of the SeDebugName privilege
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privileges.Privileges[0].Luid))
			return false;

		// adjust the token so we can have debug privileges
		privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), NULL, NULL);

		settingsMgr = new SettingsManager(processID);
		const __wchar_t __pin *projectPathStr = PtrToStringChars(projectPath);
		settingsMgr->SetProjectPath(projectPathStr);

		if (!scheduledTestWorkerRunning)
		{
			scheduledTestQueueEvent = new Event();
			scheduledTestQueueEvent->Create(NULL, false, false);
			scheduledTestQueueCompleteEvent = new Event();
			scheduledTestQueueCompleteEvent->Create(NULL, false, false);
			scheduledTestWorkerRunning = true;
			scheduledTestWorkerThread = new Thread(new ThreadStart(this, ScheduledTestWorkerThreadFunction));
			scheduledTestWorkerThread->IsBackground = true;
			scheduledTestWorkerThread->Start();
		}
		
		//Set the list of external dotnet libs that should be initialized by Kernel32ReplacementLibrary
		if (externalDotNetInterceptionLibs && externalDotNetInterceptionLibs->GetLength() > 0)
			settingsMgr->SetExternalDotNetLibsForInit (this->externalDotNetInterceptionLibs);

		logReader->ProcessID = processID;
		logReceiver = new LogReceiver(processID);
		logReceiver->AddLogReceivedListener(logReader);
		logReceiver->StartReceivingLogs();

		childProcListener = new ChildProcessListener(processID);
		childProcListener->OnChildProcessCreated += new ChildProcessCreatedDelegate(this, onChildProcessCreated);
		childProcListener->StartListeningForChildProcesses();

		char pidStr[10];
		itoa(processID, pidStr, 10);

		String *cantCreateLogFileEventName = String::Concat(CANT_CREATE_LOG_FILE_PREFIX, pidStr);
		const __wchar_t __pin *cantCreateLogFileName = PtrToStringChars(cantCreateLogFileEventName);

		cantCreateLogFileEvent = new Event();
		cantCreateLogFileEvent->Create(cantCreateLogFileName);
		threadRunning = true;
		ThreadStart *cantCreateLogFileThreadStart = new ThreadStart(this, CantCreateLogFileThreadFunction);
		cantCreateLogFileListeningThread = new Thread(cantCreateLogFileThreadStart);
		cantCreateLogFileListeningThread->IsBackground = true;
		cantCreateLogFileListeningThread->Start();

		// enable all the functions if not in exporter mode
		if (!UseExternalInterceptionList)
		{
			BeginFunctionLoggingUpdate();

			InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			Array *functions = db->Functions;
			IEnumerator *funcEnum = functions->GetEnumerator();
			while (funcEnum->MoveNext())
			{
				InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(funcEnum->Current);
				if (func != NULL)
					EnableFunctionLogging(func->Category, func->Name);
			}

			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			functions = db->Functions;
			funcEnum = functions->GetEnumerator();
			while (funcEnum->MoveNext())
			{
				InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(funcEnum->Current);
				if (func != NULL)
					EnableFunctionLogging(func->Category, func->Name);
			}

			EndFunctionLoggingUpdate();
		}

		// disable the disabled functions
		if (UseExternalInterceptionList)
			this->FilteredFunctionList = this->GetFilteredFunctionList();
		else
			this->FilteredFunctionList = filteredFunctionList;

		String *logFileName = "";

		// determine the path and name of the logfile		
		if (!UseExternalInterceptionList)
		{
			// get the install path
			String *logFolderName;
			RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			logFileName = static_cast<String *>(key->GetValue(INSTALL_PATH_VALUE_NAME));
			logFolderName = static_cast<String *>(key->GetValue(LOG_PATH_VALUE_NAME));
			key->Close();

			// make sure the log folder entry is not null
			if (logFolderName == NULL)
				logFolderName = LOG_FOLDER_DEFAULT_VALUE;

			// open the log file
			logFileName = String::Concat(logFileName, "\\", logFolderName);
		}

		// create the directory if it doesn't exist
		const __wchar_t __pin *fullLogFolderName = PtrToStringChars(logFileName);
		CreateDirectoryW(fullLogFolderName, NULL);

		logFileName = String::Concat(logFileName, "\\", pidStr, ".hdl");
		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);

		logReader->LogFileName = logFileName;

		// don't terminate this process on exit
		appNeedsToBeTerminated = false;

		// actually attach the application
		if (suspendedApp)
		{
			if (heat->attachToApp(processID) != HEAT_SUCCESS)
			{
				if (settingsMgr != NULL)
					delete settingsMgr;
				settingsMgr = NULL;

				delete logReceiver;
				logReceiver = NULL;

				return false;
			}
		}
		else
		{
			if (heat->attachToRunningApp(processID) != HEAT_SUCCESS)
			{
				if (settingsMgr != NULL)
					delete settingsMgr;
				settingsMgr = NULL;

				delete logReceiver;
				logReceiver = NULL;

				return false;
			}
		}

		if (!testPauseWatchThreadRunning)
		{
			testPauseWatchThreadRunning = true;
			testPauseWatchThread = new Thread(new ThreadStart(this, TestPauseWatchThreadFunction));
			testPauseWatchThread->Start();
		}

		StartProcessWatching();

		heat->deInitOnProcessTerminate(GetCurrentProcessId());

		// if the user wants to pause the application, pause it
		if (pauseOnAttach)
			heat->pauseApp();

		// Attach the debugger. With W2k3 SP1, attaching the debugger after HeapAlloc 
		// has been intercepted sometimes caused a crash. So it is important that we 
		// attach the debugger _before_ sending the intercepts
		if (attachDebugger)
			AttachDebuggerToProcess();

		if (injectOnlyServiceExeLibrary)
		{
			RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		
			String *installPath, *pluginFolder;
			installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
			pluginFolder = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));

			String *pluginPath = String::Concat(installPath, "\\", pluginFolder);

			const int stringSize = 511;
			char name[stringSize + 1], originalDll[stringSize + 1];
			char replacementName[stringSize + 1], interceptedDll[stringSize + 1];
			String *fullLibraryPath = NULL;

			fullLibraryPath = String::Concat(pluginPath, "\\ServicesExeReplacementLibrary.dll");

			const __wchar_t *wideNames[] = 
			{
				L"CreateProcessA",
				L"CreateProcessW",
				L"CreateProcessAsUserA",
				L"CreateProcessAsUserW",
				L"CreateProcessWithLogonW",
				L"CreateProcessWithTokenW"
			};

			const __wchar_t *wideDlls[] =
			{
				L"kernel32.dll",
				L"kernel32.dll",
				L"advapi32.dll",
				L"advapi32.dll",
				L"advapi32.dll",
				L"advapi32.dll"
			};

			const __wchar_t *wideReplacementNames[] =
			{
				L"CreateProcessAReplacement",
				L"CreateProcessWReplacement",
				L"CreateProcessAsUserAReplacement",
				L"CreateProcessAsUserWReplacement",
				L"CreateProcessWithLogonWReplacement",
				L"CreateProcessWithTokenWReplacement",
			};

			SiString str;
			Holodeck::StringConverter::GetInstance()->ConvertToSiString(fullLibraryPath, &str);
			const __wchar_t *wideInterceptedDll = str;
			wcstombs(interceptedDll, wideInterceptedDll, stringSize);

			int numFunctions = sizeof(wideReplacementNames) / sizeof(char *);
			for (int funcIndex = 0; funcIndex < numFunctions; funcIndex++)
			{
				wcstombs(name, wideNames[funcIndex], stringSize);
				wcstombs(originalDll, wideDlls[funcIndex], stringSize);
				wcstombs(replacementName, wideReplacementNames[funcIndex], stringSize);

				heat->interceptFunc(name, originalDll, replacementName, interceptedDll);
			}
		}
		else
		{
			if (UseExternalInterceptionList)
				this->InterceptFunctionsFromInterceptionList();
			else
				this->InterceptFunctionsFromDatabase();
		}

		if (heat->forceInject() != HEAT_SUCCESS)
		{
			if (settingsMgr != NULL)
				delete settingsMgr;
			settingsMgr = NULL;

			delete logReceiver;
			logReceiver = NULL;

			return false;
		}

		logReader->CreateLogFile(fullLogFileName, pidStr);

		// if the user does not want to keep the application paused, resume it
		if (!pauseOnAttach)
			heat->resumeApp();

		return true;
	}

	//*************************************************************************
	// Method:		GetProcessIdForProcessName
	// Description: gets the process id of a particular process
	//
	// Parameters:
	//	procNameToFind - the exe name of the process to get the pid for
	//
	// Return Value: >= 0 if successful, -1 otherwise
	//*************************************************************************
	int HoloScriptApplication::GetProcessIdForProcessName(String *procNameToFind)
	{
		System::Diagnostics::Process *processList[] = System::Diagnostics::Process::GetProcesses();
		System::Diagnostics::Process *currentProcess;
		
		for (int i=0; i<processList->Length; i++)
		{
			try
			{
				currentProcess = dynamic_cast <System::Diagnostics::Process*> (processList->GetValue(i));
				String *procName = currentProcess->ProcessName;
				if (procName->ToLower()->CompareTo(procNameToFind->ToLower()) == 0)
					return currentProcess->Id;
			}
			catch(...)
			{
			}
		}
		return -1;
	}

	//*************************************************************************
	// Method:		LaunchService
	// Description: Starts a service with HoloScript attached to it
	//
	// Parameters:
	//	serviceToLaunch - the name of the service to start
	//	pauseOnStart - true to pause the app on start, false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::LaunchService(String *serviceToLaunch, bool pauseOnStart, ArrayList *filteredFunctionList)
	{
		isService = true;
		serviceName = serviceToLaunch;
		appNeedsToBeTerminated = true;
		appAttachedTo = false;

		AttachToServicesExe();
		ServiceHelper *helper = new ServiceHelper();

		ServiceController *service = new ServiceController(serviceToLaunch);
		if (!service)
			return false;

		if (service->Status != ServiceControllerStatus::Stopped)
		{
			throw new Exception("Please wait for the service to be stop before launching it");
		}

		String *originalGroup;
		String *newGroup;
		bool newGroupCreatedSuccessfully = false;
		if (service->ServiceType == ServiceType::Win32ShareProcess)
		{
			// create the new shared service group
			originalGroup = helper->GetCurrentSharedServiceGroup(service->ServiceName);
			if (originalGroup)
			{
				newGroup = helper->CreateNewServiceGroup(originalGroup, service->ServiceName);
				if (newGroup)
				{
					helper->ChangeServiceImagePathToUseGroup(originalGroup, newGroup, service->ServiceName);
					newGroupCreatedSuccessfully = helper->DeleteServiceFromGroup(originalGroup, service->ServiceName);
				}
			}
		}

		waitForLaunchedServiceEvent->Release();
		if (service->ServiceType == ServiceType::Win32ShareProcess)
		{
			service->Start();
		}
		else
		{
			serviceToLaunchInAThread = serviceName;
			ThreadStart *serviceStartThreadStart = new ThreadStart(this, StartServiceThreadFunction);
			Thread *serviceStartThread = new Thread(serviceStartThreadStart);
			serviceStartThread->IsBackground = true;
			serviceStartThread->Start();
		}
			
		unsigned int result = waitForLaunchedServiceEvent->Acquire(WAIT_FOR_SERVICE_TIMEOUT);
		unsigned int processID = launchedServiceProcessId;

		launchedServiceProcessId = 0;
		serviceToLaunchInAThread = NULL;

		if (service->ServiceType == ServiceType::Win32ShareProcess)
		{
			// remove the new shared service group
			if (newGroupCreatedSuccessfully)
			{
				helper->AddServiceToGroup(originalGroup, service->ServiceName);
				helper->ChangeServiceImagePathToUseGroup(newGroup, originalGroup, service->ServiceName);
				helper->DeleteServiceGroup(newGroup);
			}
		}

		if (result == EVENT_TIMED_OUT)
			return false;

		bool retVal = false;
		if (service->ServiceType == ServiceType::Win32ShareProcess)
		{
			retVal = AttachApplication(processID, pauseOnStart, filteredFunctionList, false);
		}
		else
		{
			retVal = AttachApplication(processID, pauseOnStart, filteredFunctionList, true);
			readyToContinueLaunchEvent->Signal();
		}

		appNeedsToBeTerminated = true;
		appAttachedTo = false;
		serviceName = serviceToLaunch;
		isService = true;
		this->pauseOnStart = pauseOnStart;

		return retVal;
	}

	//**************************************************************************
	// Method:		StartServiceThreadFunction
	// Description: main thread function for the thread that launches the service
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::StartServiceThreadFunction()
	{
		try
		{
			ServiceController *service = new ServiceController(serviceToLaunchInAThread);
			if (service)
				service->Start();
		}
		catch(...)
		{
		}
	}

	//**************************************************************************
	// Method:		onAttachToLaunchedService
	// Description: called when a service child process needs to be attached to
	//
	// Parameters:
	//  parentPid - the parent process of the new process
	//	pid - the process id to attach to
	//	resumeApp - true if we are supposed to resume the aut after attach, false
	//		otherwise
	//
	// Return Value: none
	//*************************************************************************
	void HoloScriptApplication::onAttachToLaunchedService(unsigned long parentPid, unsigned long pid, bool resumeApp)
	{
		if (pid != 0)
		{
			if (serviceToLaunchInAThread != NULL)
			{
				readyToContinueLaunchEvent->Release();

				launchedServiceProcessId = pid;
				waitForLaunchedServiceEvent->Signal();

				readyToContinueLaunchEvent->Acquire(WAIT_FOR_SERVICE_TIMEOUT);
			}
			else
			{
				launchedServiceProcessId = pid;
				waitForLaunchedServiceEvent->Signal();
			}
		}
	}

	//*************************************************************************
	// Method:		AttachToServicesExe
	// Description: Attaches to services.exe
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::AttachToServicesExe()
	{
		HoloScriptRegistry *hsReg = HoloScriptRegistry::getInstance();
		if (hsReg->ServicesExeApplication == NULL)
		{
			HoloScriptApplication *servicesExeApp = new HoloScriptApplication();
			servicesExeApp->OnAttachToChildProcess += new AttachToChildProcessDelegate(this, onAttachToLaunchedService);

			// get the pid of services.exe
			int servicesExeProcId = servicesExeApp->GetProcessIdForProcessName("services");
			if (servicesExeProcId == -1)
				return;

			if (!servicesExeApp->AttachApplication(servicesExeProcId, true, new ArrayList(), NULL, false, false, true))
				return;

			servicesExeApp->ProcessChaining = true;
			hsReg->ServicesExeApplication = servicesExeApp;

			hsReg->addHoloScriptAppForProcess(servicesExeProcId, servicesExeApp);

			servicesExeApp->ResumeApplication();
		}
	}

	//*************************************************************************
	// Method:		SetApplicationName
	// Description: gets the full path to the main module of the process
	//
	// Parameters:
	//	pid - the process id of the app
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetApplicationName(unsigned long pid)
	{
		HMODULE mods[1024];
		HANDLE process;
		DWORD bytesNeeded;
		bool retVal = false;

		// Get a list of all the modules in this process.
		process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (!process)
			return false;

		if (EnumProcessModules(process, mods, sizeof(mods), &bytesNeeded))
		{
			if ((bytesNeeded / sizeof(HMODULE)) > 0)
			{
				char modName[1024];

				// Get the full path to the module's file.
				if (GetModuleFileNameEx(process, mods[0], modName, sizeof(modName)))
				{
					applicationToLaunch = new String(modName);
					retVal = true;

				}
			}
		}

		CloseHandle(process);
		return retVal;
	}
	
	//*************************************************************************
	// Method:		AttachService
	// Description: Attaches HoloScript to a running service's process
	//
	// Parameters:
	//	serviceToAttachTo - the name of the service to attach to
	//	pauseOnAttach - true if you want to pause the application on attachment,
	//		false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//	terminateOnExit - true to terminate the app on exit, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::AttachService(String *serviceToAttachTo, bool pauseOnAttach, ArrayList *filteredFunctionList, bool terminateOnExit)
	{
		ServiceController *service = new ServiceController(serviceToAttachTo);
		if (!service)
			return false;

		if (service->Status != ServiceControllerStatus::Running)
		{
			throw new Exception("Please wait for the service to be running before attaching to it");
		}

		try
		{
			ServiceHelper *helper = new ServiceHelper();
			unsigned long pid = helper->GetProcessIDOfRunningService(serviceToAttachTo);
			if (pid == 0)
				return false;

			isService = true;
			serviceName = serviceToAttachTo;
			appAttachedTo = true;

			bool result = AttachApplication(pid, pauseOnAttach, filteredFunctionList, false);
			appNeedsToBeTerminated = terminateOnExit;
			isService = true;
			serviceName = serviceToAttachTo;
			this->pauseOnStart = pauseOnAttach;
			return result;
		}
		catch(...)
		{
		}

		return false;
	}

	//*************************************************************************
	// Method:		set_FilteredFunctionList
	// Description: Sets filters based on a filtered function name list
	//
	// Parameters:
	//	value - array of strings representing function names to filter
	//
	// Return Value: none
	//*************************************************************************
	void HoloScriptApplication::set_FilteredFunctionList(ArrayList *value)
	{
		BeginFunctionLoggingUpdate();

		InterceptedFunctionDB *nativeDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);

		Hashtable* filteredHash = new Hashtable();
		for (int i=0; i < value->Count; i++)
		{
			String* name = dynamic_cast <String*> (value->Item[i]);
			if (!filteredHash->Contains(name))
				filteredHash->Add(name, __box(true));
		}

		// Go through and enable the functions that aren't disabled.  Enumerate the native functions first
		IEnumerator *enumerator = nativeDB->Functions->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunction *function = dynamic_cast<InterceptedFunction*>(enumerator->Current);
			if (!filteredHash->ContainsKey(function->Name))
				EnableFunctionLogging(function->Category, function->Name);
			else
				DisableFunctionLogging(function->Category, function->Name);
		}

		// Now check the .NET functions
		enumerator = dotNetDB->Functions->GetEnumerator();
		while (enumerator->MoveNext())
		{
			InterceptedFunction *function = dynamic_cast<InterceptedFunction*>(enumerator->Current);
			if (!filteredHash->ContainsKey(function->Name))
				EnableFunctionLogging(function->Category, function->Name);
			else
				DisableFunctionLogging(function->Category, function->Name);
		}
		
		filteredFunctionList = value;

		EndFunctionLoggingUpdate();
	}

	//*************************************************************************
	// Method:		LaunchApplication
	// Description: launches an application and attaches HoloScript to it
	//
	// Parameters:
	//	applicationToLaunch - the full path to the executable to launch
	//  commandLineParams - command line params to pass to the app
	//	pauseOnStart - true if you want to pause the application on start,
	//		false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::LaunchApplication(String *ApplicationToLaunch, String *CommandLineParams, bool PauseOnStart, ArrayList *filteredFunctionList)
	{
		return this->LaunchApplication (ApplicationToLaunch, CommandLineParams, PauseOnStart, filteredFunctionList, NULL, false);
	}

	//*************************************************************************
	// Method:		LaunchApplication
	// Description: launches an application and attaches HoloScript to it
	//
	// Parameters:
	//	applicationToLaunch - the full path to the executable to launch
	//  commandLineParams - command line params to pass to the app
	//	pauseOnStart - true if you want to pause the application on start,
	//		false otherwise
	//	filteredFunctionList - the list of function name strings to disable
	//	exporterLog	- the path and name of the file to use for logging (if NULL, default is used)
	//	UseExternalInterceptionList - used to signify that the LibraryExporter is using the function
	//				and interception of functions should be performed from a different interception list
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::LaunchApplication(String *ApplicationToLaunch, String *CommandLineParams, bool PauseOnStart, ArrayList *filteredFunctionList, String * exporterLog, bool UseExternalInterceptionList)
	{
		//If FilteredFunctionList is empty then create an emply list
		if (filteredFunctionList == NULL)
			filteredFunctionList = new ArrayList();

		appAttachedTo = false;
		isService = false;
		serviceName = NULL;

		PROCESS_INFORMATION procInfo;
		STARTUPINFOW startupInfo;
		String *fullAppPath;

		if (IsSystemProcess(ApplicationToLaunch))
		{
			throw new Exception("This is an important system process that should not be launched under Holodeck.");
		}

		applicationToLaunch = ApplicationToLaunch;
		commandLineParams = CommandLineParams;
		pauseOnStart = PauseOnStart;

		// initialize the startup info structure
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		// Get the starting AUT current directory
		wchar_t curDirDrive[_MAX_DRIVE], curDirFolder[_MAX_DIR];
		wchar_t curDirPath[MAX_PATH];
		const __wchar_t __pin *appToLaunch = PtrToStringChars(applicationToLaunch);
		_wsplitpath(appToLaunch, curDirDrive, curDirFolder, NULL, NULL);
		wcscpy(curDirPath, curDirDrive);
		wcscat(curDirPath, curDirFolder);

		fullAppPath = String::Concat(applicationToLaunch, " ");
		fullAppPath = String::Concat(fullAppPath, CommandLineParams);
		const __wchar_t __pin *appPath = PtrToStringChars(fullAppPath);
		if (CreateProcessW(NULL, appPath, NULL, NULL, false, CREATE_SUSPENDED, NULL, curDirPath, &startupInfo, &procInfo) == 0)
			return false;

		settingsMgr = new SettingsManager(procInfo.dwProcessId);
		const __wchar_t __pin *projectPathStr = PtrToStringChars(projectPath);
		//Set the project path
		settingsMgr->SetProjectPath(projectPathStr);
		//Set the list of external dotnet libs that should be initialized by Kernel32ReplacementLibrary
		if (externalDotNetInterceptionLibs && externalDotNetInterceptionLibs->GetLength() > 0)
			settingsMgr->SetExternalDotNetLibsForInit (this->externalDotNetInterceptionLibs);

		if (!scheduledTestWorkerRunning)
		{
			scheduledTestQueueEvent = new Event();
			scheduledTestQueueEvent->Create(NULL, false, false);
			scheduledTestQueueCompleteEvent = new Event();
			scheduledTestQueueCompleteEvent->Create(NULL, false, false);
			scheduledTestWorkerRunning = true;
			scheduledTestWorkerThread = new Thread(new ThreadStart(this, ScheduledTestWorkerThreadFunction));
			scheduledTestWorkerThread->IsBackground = true;
			scheduledTestWorkerThread->Start();
		}
		
		logReader->ProcessID = procInfo.dwProcessId;
		logReceiver = new LogReceiver(procInfo.dwProcessId);
		logReceiver->AddLogReceivedListener(logReader);
		logReceiver->StartReceivingLogs();

		childProcListener = new ChildProcessListener(procInfo.dwProcessId);
		childProcListener->OnChildProcessCreated += new ChildProcessCreatedDelegate(this, onChildProcessCreated);
		childProcListener->StartListeningForChildProcesses();

		char pidStr[10];
		_itoa(procInfo.dwProcessId, pidStr, 10);

		String *cantCreateLogFileEventName = String::Concat(CANT_CREATE_LOG_FILE_PREFIX, pidStr);
		const __wchar_t __pin *cantCreateLogFileName = PtrToStringChars(cantCreateLogFileEventName);

		cantCreateLogFileEvent = new Event();
		cantCreateLogFileEvent->Create(cantCreateLogFileName);
		threadRunning = true;
		ThreadStart *cantCreateLogFileThreadStart = new ThreadStart(this, CantCreateLogFileThreadFunction);
		cantCreateLogFileListeningThread = new Thread(cantCreateLogFileThreadStart);
		cantCreateLogFileListeningThread->IsBackground = true;
		cantCreateLogFileListeningThread->Start();

		// enable all the function logging, if not in exporter mode
		if (!UseExternalInterceptionList)
		{
			BeginFunctionLoggingUpdate();

			InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			Array *functions = db->Functions;
			IEnumerator *funcEnum = functions->GetEnumerator();
			while (funcEnum->MoveNext())
			{
				InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(funcEnum->Current);
				if (func != NULL)
					EnableFunctionLogging(func->Category, func->Name);
			}

			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			functions = db->Functions;
			funcEnum = functions->GetEnumerator();
			while (funcEnum->MoveNext())
			{
				InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(funcEnum->Current);
				if (func != NULL)
					EnableFunctionLogging(func->Category, func->Name);
			}

			EndFunctionLoggingUpdate();
		}

		// disable the disabled functions
		if (UseExternalInterceptionList)
			this->FilteredFunctionList = this->GetFilteredFunctionList();
		else
			this->FilteredFunctionList = filteredFunctionList;

		String *logFileName = "";

		// determine the path and name of the logfile		
		if (!UseExternalInterceptionList)
		{
			// get the install path
			String *logFolderName;
			RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			logFileName = static_cast<String *>(key->GetValue(INSTALL_PATH_VALUE_NAME));
			logFolderName = static_cast<String *>(key->GetValue(LOG_PATH_VALUE_NAME));
			key->Close();

			// make sure the log folder entry is not null
			if (logFolderName == NULL)
				logFolderName = LOG_FOLDER_DEFAULT_VALUE;

			// open the log file
			logFileName = String::Concat(logFileName, "\\", logFolderName);

		}

		// create the directory if it doesn't exist
		const __wchar_t __pin *fullLogFolderName = PtrToStringChars(logFileName);
		CreateDirectoryW(fullLogFolderName, NULL);

		logFileName = String::Concat(logFileName, "\\", pidStr, ".hdl");

		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);

		logReader->LogFileName = logFileName;

		// attach to the application
		if (heat->attachToApp(procInfo.dwProcessId) != HEAT_SUCCESS)
			return false;
		
		if (UseExternalInterceptionList)
			this->InterceptFunctionsFromInterceptionList();
		else
			this->InterceptFunctionsFromDatabase();

		// force the injection of the libraries
		if (heat->forceInject() != HEAT_SUCCESS)
		{
			if (settingsMgr != NULL)
				delete settingsMgr;
			settingsMgr = NULL;

			delete logReceiver;
			logReceiver = NULL;

			return false;
		}

		heat->deInitOnProcessTerminate(GetCurrentProcessId());

		//Create the log file only for Holodeck projects, not for Custom test projects
		if (!UseExternalInterceptionList)
			logReader->CreateLogFile(fullLogFileName, pidStr);

		if (!testPauseWatchThreadRunning)
		{
			testPauseWatchThreadRunning = true;
			testPauseWatchThread = new Thread(new ThreadStart(this, TestPauseWatchThreadFunction));
			testPauseWatchThread->Start();
		}

		StartProcessWatching();

		if (attachDebugger)
			AttachDebuggerToProcess();

		// if the user does not want to keep the application paused, resume it
		if (!pauseOnStart)
		{
			heat->resumeApp();
		}

		return true;
	}

	//*************************************************************************
	// Method:		PauseApplication
	// Description: Pauses the application being monitored.  Should only be
	//	called after either LaunchApplication or AttachApplication.
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::PauseApplication()
	{
		if (this->isService)
		{
			try
			{
				ServiceController *service = new ServiceController(serviceName);
				if ((!service) || (service->Status != ServiceControllerStatus::Running))
					return false;

				service->Pause();
			}
			catch (...)
			{
				MessageBox (NULL, "This service cannot be paused.", "Holodeck", 0);
				return false;
			}

			SetAppPauseState (true);
		}
		else
		{
			if (heat->pauseApp() != HEAT_SUCCESS)
				return false;
		}
		
		return true;
	}

	//*************************************************************************
	// Method:		ResumeApplication
	// Description: Resumes a paused application.
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::ResumeApplication()
	{
		try
		{
			//make sure all tests are set
			scheduledTestQueueEvent->Signal();
			scheduledTestQueueCompleteEvent->Acquire(5000);

			//firstResume flag is used to resume from the initial process pause (PauseOnStart option)
			//as compared to the Service pause
			if (this->isService && !this->firstResume)
			{ 
				ServiceController *service = new ServiceController(serviceName);
				if ((!service) || (service->Status != ServiceControllerStatus::Paused))
					return false;

				try
				{
					service->Continue();
				}
				catch(...)
				{
				}

				SetAppPauseState (false);
			}
			else
			{
				this->firstResume = false;
				if (heat->resumeApp() != HEAT_SUCCESS)
					return false;
			}
		}
		catch(...)
		{
			return false;
		}

		try
		{
			//enumerate symbols from process before injecting our interception modules
			if (!this->triedGettingSymbols && this->processSymbolsArray == NULL)
			{
				DWORD procID = heat->getProcessId();
				HANDLE hProc = OpenProcess (PROCESS_ALL_ACCESS, TRUE, procID);
				this->EnumerateSymbols (hProc);
				CloseHandle (hProc);
			}
		}
		catch(...)
		{
			// do nothing
		}

		return true;
	}

	//*************************************************************************
	// Method:		SetAppPauseState
	// Description: Manually sets the application pause state
	//
	// Parameters:
	//	value - true if paused; false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::SetAppPauseState(bool value)
	{
		heat->setPauseState(value);
	}

	//*************************************************************************
	// Method:		StopServiceThreadFunction
	// Description: stops a launched service
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	void HoloScriptApplication::StopServiceThreadFunction()
	{
		try
		{
			ServiceController *service = new ServiceController(serviceName);
			if ((!service) || (service->Status != ServiceControllerStatus::StopPending) || (service->Status == ServiceControllerStatus::Stopped))
			{
				waitForServiceStopEvent->Signal();
				return;
			}
			service->Stop();

			waitForServiceStopEvent->Signal();
		}
		catch(...)
		{
			waitForServiceStopEvent->Signal();
		}
	}

	//*************************************************************************
	// Method:		TerminateApplication
	// Description: Terminates the process being monitored.
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::TerminateApplication()
	{
		if (OnHeatProcessTerminated)
			OnHeatProcessTerminated->Invoke(heat->getProcessId());
	
		CleanUp();
	
		return true;
	}

	//*************************************************************************
	// Method:		GetApplicationProcessID
	// Description: Gets the process ID of the process being monitored
	//
	// Parameters:
	//	processID - stores the id of the running process
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetApplicationProcessID(UInt32 &processID)
	{
		if (!savedLog)
		{
			if (heat == NULL)
				return false;
            processID = UInt32(heat->getProcessId());
		}
		else
		{
			int slashIndex = logReader->LogFileName->LastIndexOf("\\");
			slashIndex += 2;
			int len = logReader->LogFileName->Length-4 - slashIndex;
			processID = Convert::ToUInt32(logReader->LogFileName->Substring(slashIndex, len));
		}

		return true;
	}	
	
	//*************************************************************************
	// Method:		GetApplicationThreadID
	// Description: Gets the thread ID of the process being monitored
	//
	// Parameters:
	//	threadID - stores the id of the running thread
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetApplicationThreadID(UInt32 &threadID)
	{
		if (!savedLog)
		{
			if (heat == NULL)
				return false;
            threadID = UInt32(heat->getThreadId());
		}
		else
			threadID = 0;

		return true;
	}


	//*************************************************************************
	// Method:		GetMonitorInformation
	// Description: Gets information about the disk and memory usage of the
	//	application being monitored.
	//
	// Parameters:
	//	maximumDiskUsage - stores the maximum limit to the disk space allocated to
	//		the application being monitored.
	//	currentDiskUsage - stores the current amount of disk space being used by the
	//		application being monitored
	//	maximumMemoryUsage - stores the maximum limit to the memory allocated to the
	//		application being monitored
	//	currentMemoryUsage - stores the current amount of memory being used by
	//		the application being monitored.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetMonitorInformation(UInt64 &maximumDiskUsage, UInt64 &currentDiskUsage, UInt64 &maximumMemoryUsage, UInt64 &currentMemoryUsage)
	{
		UInt64 maxDisk = 0, currentDisk = 0, maxMem = 0, currentMem = 0;

		if (settingsMgr == NULL)
			return false;

		//if the maximum resource key isn't available we grab system information instead.
		if (!settingsMgr->GetMaximumDiskUsage(maxDisk))
		{
			ULARGE_INTEGER i64freeDisk, i64maxDisk, i64totalFree;
			GetDiskFreeSpaceEx(NULL, (PULARGE_INTEGER) &i64freeDisk, (PULARGE_INTEGER) &i64maxDisk, (PULARGE_INTEGER) &i64totalFree);
			maximumDiskUsage = i64freeDisk.QuadPart;
		}
		else
			maximumDiskUsage = maxDisk;

		if (!settingsMgr->GetCurrentDiskUsage(currentDisk))
			currentDiskUsage = 0;
		else
			currentDiskUsage = currentDisk;

		//if the maximum resource key isn't available we grab system information instead.
		if (!settingsMgr->GetMaximumMemoryUsage(maxMem))
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof (statex);
			GlobalMemoryStatusEx (&statex);
			maximumMemoryUsage = statex.ullAvailVirtual;
		}
		else
			maximumMemoryUsage = maxMem;

		if (!settingsMgr->GetCurrentMemoryUsage(currentMem))
			currentMemoryUsage = 0;
		else
			currentMemoryUsage = currentMem;
		
		return true;
	}


	//*************************************************************************
	// Method:		GetNetworkMonitorInformation
	// Description: Gets information about the network usage
	//
	// Parameters:
	//	uploadUsage - receives the upload usage information in bytes/second
	//	downloadUsage - receives the download usage information in bytes/second
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetNetworkMonitorInformation (UInt64 &UploadUsage, UInt64 &DownloadUsage)
	{
		UInt64 totalUploadBytes = 0, totalDownloadBytes = 0;
		DWORD uploadTimeStamp = 0, downloadTimeStamp = 0, currentTime;

		// Keep a circular buffer of 5 entries to average the bandwidth measurement over the last 5 seconds
		static UINT64 oldTotalUploadBytes[5] = {0, 0, 0, 0, 0};
		static UINT64 oldTotalDownloadBytes[5] = {0, 0, 0, 0, 0};
		static DWORD oldUploadTimeStamp[5] = {0, 0, 0, 0, 0};
		static DWORD oldDownloadTimeStamp[5] = {0, 0, 0,0 ,0};
		static DWORD oldUploadIndex = 0;
		static DWORD oldDownloadIndex = 0;

		if (settingsMgr == NULL)
			return false;

		currentTime = GetTickCount();
		if (settingsMgr->GetUploadBandwidthUsage(totalUploadBytes, uploadTimeStamp))
		{
			//timestamp only updates if data is sent, so if timestamp is stale update it.
			if ( (currentTime - uploadTimeStamp) >= 2000)
				uploadTimeStamp = currentTime;

			if ( (uploadTimeStamp - oldUploadTimeStamp[oldUploadIndex]) <= 0)
				UploadUsage = 0;
			else
			{
				UploadUsage = ((totalUploadBytes - oldTotalUploadBytes[oldUploadIndex]) * 1000) /
					(uploadTimeStamp - oldUploadTimeStamp[oldUploadIndex]);
			}
			
			//limit average to the last 5 seconds
			while ( (uploadTimeStamp - oldUploadTimeStamp[oldUploadIndex]) >= 5000)
			{
				// Place current data in the circular buffer and move to next entry
				oldTotalUploadBytes[oldUploadIndex] = totalUploadBytes;
				oldUploadTimeStamp[oldUploadIndex] = uploadTimeStamp;
				oldUploadIndex = (oldUploadIndex + 1) % 5;
			}
		}

		if (settingsMgr->GetDownloadBandwidthUsage(totalDownloadBytes, downloadTimeStamp))
		{
			//timestamp only updates if data is recieved, so if timestamp is stale update it.
			if ( (currentTime - downloadTimeStamp) >= 2000)
				downloadTimeStamp = currentTime;

			if ( (downloadTimeStamp - oldDownloadTimeStamp[oldDownloadIndex]) <= 0)
				DownloadUsage = 0;
			else
			{
				DownloadUsage = ((totalDownloadBytes - oldTotalDownloadBytes[oldDownloadIndex]) * 1000) /
					(downloadTimeStamp - oldDownloadTimeStamp[oldDownloadIndex]);
			}
			
			//limit average to the last 5 seconds
			if ( (downloadTimeStamp - oldDownloadTimeStamp[oldDownloadIndex]) >= 5000)
			{
				// Place current data in the circular buffer and move to next entry
				oldTotalDownloadBytes[oldDownloadIndex] = totalDownloadBytes;
				oldDownloadTimeStamp[oldDownloadIndex] = downloadTimeStamp;
				oldDownloadIndex = (oldDownloadIndex + 1) % 5;
			}
		}
		
		return true;
	}

	//*************************************************************************
	// Method:		GetRealSpaceInformation
	// Description: returns max disk and memory space regardless of limits set
	//
	// Parameters:
	//	maximumDiskUsage - stores the maximum limit to the disk space allocated to
	//		the application being monitored.
	//	maximumMemoryUsage - stores the maximum limit to the memory allocated to the
	//		application being monitored
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetRealSpaceInformation(UInt64 &maximumDiskUsage, UInt64 &maximumMemoryUsage)
	{

		ULARGE_INTEGER i64freeDisk, i64maxDisk, i64totalFree;
		GetDiskFreeSpaceEx(NULL, (PULARGE_INTEGER) &i64freeDisk, (PULARGE_INTEGER) &i64maxDisk, (PULARGE_INTEGER) &i64totalFree);
		maximumDiskUsage = i64freeDisk.QuadPart;


		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx (&statex);
		maximumMemoryUsage = statex.ullAvailVirtual;
		
		return true;
	}

	//*************************************************************************
	// Method:		SetMaximumDiskUsage
	// Description: Sets the maximum disk space allowable for the application
	//	under test.
	//
	// Parameters:
	//	maximumDiskUsage - the amount of disk space the application under test
	//		is allowed to use
	//	isEnabled - true if disk limits are enabled, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetMaximumDiskUsage(UInt64 maximumDiskUsage, bool isEnabled)
	{
		if (settingsMgr != NULL)
			return settingsMgr->SetMaximumDiskUsage(maximumDiskUsage, isEnabled);
		return false;
	}

	//*************************************************************************
	// Method:		SetMaximumMemoryUsage
	// Description: Sets the maximum memory allowable for the application
	//	under test.
	//
	// Parameters:
	//	maximumMemoryUsage - the amount of memory the application under test
	//		is allowed to use
	//	isEnabled - true if memory limits are enabled, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetMaximumMemoryUsage(UInt64 maximumMemoryUsage, bool isEnabled)
	{
		if (settingsMgr != NULL)
			return settingsMgr->SetMaximumMemoryUsage(maximumMemoryUsage, isEnabled);
		return false;
	}

	//*************************************************************************
	// Method:		SetNetworkUploadBandwidth
	// Description: Sets the maximum permissible speed for upload
	//
	// Parameters:
	//  maxUploadSpeed - bytes/second allowed for upload
	//	IsUploadControlEnabled - true if upload traffic control is desired, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetNetworkUploadBandwidth(UInt64 maxUploadSpeed, bool IsUploadControlEnabled)
	{
		if (settingsMgr != NULL)
			return settingsMgr->SetNetworkUploadBandwidth (maxUploadSpeed, IsUploadControlEnabled);
		return false;
	}

	//*************************************************************************
	// Method:		SetNetworkDownloadBandwidth
	// Description: Sets the maximum permissible speed for download
	//
	// Parameters:
	//  maxDownloadSpeed - bytes/second allowed for download
	//	IsDownloadControlEnabled - true if download traffic control is desired, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetNetworkDownloadBandwidth(UInt64 maxDownloadSpeed, bool IsDownloadControlEnabled)
	{
		if (settingsMgr != NULL)
			return settingsMgr->SetNetworkDownloadBandwidth (maxDownloadSpeed, IsDownloadControlEnabled);
		return false;
	}

	//*************************************************************************
	// Method:		GetNetworkUploadBandwidth
	// Description: Gets the maximum upload bandwidth allowed
	//
	// Parameters:
	//  uploadBandwidth - the allowed upload bw in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetNetworkUploadBandwidth(UInt64 &uploadBandwidth)
	{
		DWORD64 uploadValue;
		bool retValue;

		if (settingsMgr != NULL)
		{
			retValue = settingsMgr->GetNetworkUploadBandwidth (uploadValue);
			uploadBandwidth = uploadValue;
			return retValue;
		}
		return false;
	}

	//*************************************************************************
	// Method:		GetNetworkDownloadBandwidth
	// Description: Gets the maximum download bandwidth allowed
	//
	// Parameters:
	//  uploadBandwidth - the allowed download bw in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::GetNetworkDownloadBandwidth(UInt64 &downloadBandwidth)
	{
		DWORD64 downloadValue;
		bool retValue;

		if (settingsMgr != NULL)
		{
			retValue = settingsMgr->GetNetworkDownloadBandwidth (downloadValue);
			downloadBandwidth = downloadValue;
			return retValue;
		}
		return false;
	}

	//*************************************************************************
	// Method:		InjectResourceFault
	// Description: Injects a fault into the requested process/thread(s)
	//
	// Parameters:
	//	targetThreadID - thread(s) into which fault should be injected, if array
	//		is empty or null then fault is injected into entire process
	//	faultID - the fault to inject
	//	resourceName - resource this fault targets
	//
	// Return Value: list of tests created
	//*************************************************************************
	ArrayList *HoloScriptApplication::InjectResourceFault(Array *targetThreadID, int faultID, String *resourceName)
	{
		ArrayList *threadIDs = new ArrayList();
		ArrayList *testIDs = new ArrayList();
		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		String *installPath, *functionFolder;
		FaultFunction *resourceFunction;

		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!db->IsLoaded)
			db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		if (!faultDB->IsLoaded)
			faultDB->LoadDatabase(functionFolder);

		// get the fault functions
		Fault *fault = faultDB->GetFault(faultID);
		if (!fault)
			return testIDs;

		Array *faultFunctions = fault->FaultFunctions;

		try
		{
			IEnumerator *functionEnumerator = faultFunctions->GetEnumerator();
			while (functionEnumerator->MoveNext())
			{
				FaultFunction *function = __try_cast<FaultFunction *>(functionEnumerator->Current);
				resourceFunction = NULL;

				// need the real function declaration to know which category the function is in
				InterceptedFunction *interceptedFunction = db->GetFunctionByName(function->Name);

				//resource-test specific code
				if ((interceptedFunction == NULL) || (interceptedFunction->Parameters == NULL))
					continue;

				IEnumerator *paramEnumerator = interceptedFunction->Parameters->GetEnumerator();
				while (paramEnumerator->MoveNext())
				{
					InterceptedFunctionParameter *param = __try_cast<InterceptedFunctionParameter*> (paramEnumerator->Current);
					if (param->PointsToResource)
					{
						if (!resourceFunction)
						{
							resourceFunction = dynamic_cast <FaultFunction*> (function->MemberwiseClone());
							resourceFunction->TestID = NULL;
						}
						//modify to match for resource-based test
						InterceptedFunctionParameter *resourceParam = __try_cast<InterceptedFunctionParameter*> (param->MemberwiseClone());
						resourceParam->AddTestCase (new ParamTestCase (resourceName, STARTS_WITH_RESOURCE_OPERATOR));
						//since we are matching on resources it will be a string type unless already a string type or a refiid
						switch (resourceParam->CompareAsType)
						{
						case StringType:
						case WideStringType:
						case OutStringType:
						case OutWideStringType:
						case IIDRefType:
							break;
						default:
							resourceParam->CompareAsType = StringType;
							resourceParam->Type = "LPCSTR";
						}
						
						//***remove the param if it already exists
						if (resourceFunction->MatchParams)
						{
							IEnumerator *checkParamEnumerator = resourceFunction->MatchParams->GetEnumerator();
							while (checkParamEnumerator->MoveNext())
							{
								InterceptedFunctionParameter *checkParam = __try_cast<InterceptedFunctionParameter*> (checkParamEnumerator->Current);
								if (checkParam->ID == param->ID)
								{
									resourceFunction->MatchParams->Remove(checkParam);
									break;
								}
							}
						}
						//now add the param to the match list
						resourceFunction->MatchParams->Add(resourceParam);
					}
				}

				//create the test
				if (resourceFunction)
				{
					if (CreateScheduledTest(targetThreadID, interceptedFunction->Category, resourceFunction))
					{
						testIDs->Add(resourceFunction->TestID);
						resourceFunction->MatchParams->Clear();
						function->MatchParams->Clear();
					}
				}
			}
		}
		catch(...)
		{
		}

		return testIDs;
	}

	//*************************************************************************
	// Method:		RemoveResourceFault
	// Description: Removes a fault in the requested process/thread(s)
	//
	// Parameters:
	//	targetThreadID - thread(s) into which fault should be injected, if array
	//		is empty or null then fault is injected into entire process
	//	testIDs - tests to delete.  Should be the arraylist returned by InjectResourceFault
	//
	// Return Value: true if succesfull, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::RemoveResourceFault(Array *targetThreadID, ArrayList *testIDs)
	{
		if (!testIDs)
			return false;

		for (int i=0; i<testIDs->Count; i++)
		{
			DeleteScheduledTest(targetThreadID, dynamic_cast <String*> (testIDs->Item[i]));
		}

		return true;
	}

	//*************************************************************************
	// Method:		InjectFault
	// Description: Injects a fault into the requested process/thread(s)
	//
	// Parameters:
	//	targetThreadID - thread(s) into which fault should be injected, if array
	//		is empty or null then fault is injected into entire process
	//	faultID - the fault to inject
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::InjectFault(Array *targetThreadID, int faultID)
	{
		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		String *installPath, *functionFolder;

		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!db->IsLoaded)
			db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		if (!faultDB->IsLoaded)
			faultDB->LoadDatabase(functionFolder);

		// get the fault functions
		Array *faultFunctions = faultDB->GetFunctionsWithFaultID(faultID);

		try
		{
			IEnumerator *enumerator = faultFunctions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				FaultFunction *function = __try_cast<FaultFunction *>(enumerator->Current);

				// need the real function declaration to know which category the function is in
				InterceptedFunction *interceptedFunction = db->GetFunctionByName(function->Name);
				if (interceptedFunction != NULL)
				{
					CreateScheduledTest(targetThreadID, interceptedFunction->Category, function); 
				}
			}
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		RemoveFault
	// Description: Removes a fault from the process being monitored
	//
	// Parameters:
	//	targetThreadID	- thread(s) from which fault should be removed, if array
	//					  is empty or null then fault is removed from entire process
	//	faultID - the fault to remove
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::RemoveFault(Array *targetThreadID, int faultID)
	{
		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		
		String *installPath, *functionFolder;
		installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		functionFolder = String::Concat(installPath, "\\", functionFolder);

		InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!db->IsLoaded)
			db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		if (!faultDB->IsLoaded)
			faultDB->LoadDatabase(functionFolder);

		// get the fault functions
		Array *faultFunctions = faultDB->GetFunctionsWithFaultID(faultID);

		try
		{
			IEnumerator *enumerator = faultFunctions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				FaultFunction *function = __try_cast<FaultFunction *>(enumerator->Current);
				DeleteScheduledTest(targetThreadID, function->TestID);

			}
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		BeginFunctionLoggingUpdate
	// Description: Marks the start of a batch change to the list of functions
	//				being logged
	//
	// Parameters:
	//	category - The category of the function to disable logging for
	//	functionName - The name of the function to disable logging for
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	void HoloScriptApplication::BeginFunctionLoggingUpdate()
	{
		functionLoggingUpdate = true;
	}

	//*************************************************************************
	// Method:		EndFunctionLoggingUpdate
	// Description: Marks the end of a batch change to the list of functions
	//				being logged, flushing the changes to the registry
	//
	// Parameters:
	//	category - The category of the function to disable logging for
	//	functionName - The name of the function to disable logging for
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	void HoloScriptApplication::EndFunctionLoggingUpdate()
	{
		functionLoggingUpdate = false;
		if (settingsMgr != NULL)
			settingsMgr->CommitLoggingOptions();
	}

	//*************************************************************************
	// Method:		EnableFunctionLogging
	// Description: Enables logging of a specified function in a specified
	//	category
	//
	// Parameters:
	//	category - The category of the function to enable logging for
	//	functionName - The name of the function to enable logging for
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::EnableFunctionLogging(String *category, String *functionName)
	{
		const __wchar_t __pin *cat = PtrToStringChars(category);
		const __wchar_t __pin *func = PtrToStringChars(functionName);

		if (settingsMgr != NULL)
		{
			bool retVal = settingsMgr->SetFunctionLoggingEnabled(cat, func, true);
			if (!functionLoggingUpdate)
				settingsMgr->CommitLoggingOptions();
			return retVal;
		}
		return false;
	}

	//*************************************************************************
	// Method:		DisableFunctionLogging
	// Description: Disables logging of a specified function in a specified
	//	category
	//
	// Parameters:
	//	category - The category of the function to disable logging for
	//	functionName - The name of the function to disable logging for
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::DisableFunctionLogging(String *category, String *functionName)
	{
		if (!category || !functionName)
			return false;

		const __wchar_t __pin *cat = PtrToStringChars(category);
		const __wchar_t __pin *func = PtrToStringChars(functionName);

		if (settingsMgr != NULL)
		{
			bool retVal = settingsMgr->SetFunctionLoggingEnabled(cat, func, false);
			if (!functionLoggingUpdate)
				settingsMgr->CommitLoggingOptions();
			return retVal;
		}
		return false;
	}

	//*************************************************************************
	// Method:		SetUseProcessChaining
	// Description: Sets whether or not to chain interception and logging of
	//	an application under test to its child processes
	//
	// Parameters:
	//	useChaining - true to use process chaining, false otherwise
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetUseProcessChaining(bool useChaining)
	{
		if (settingsMgr != NULL)
			return settingsMgr->SetProcessChainingEnabled(useChaining);
		return false;
	}

	//*************************************************************************
	// Method:		GetScheduledTestExecutionCount
	// Description: Gets the execution count for the requested scheduled test
	//
	// Parameters:
	//	targetThreadID - the threadID for which the test exists
	//	category - the category of the test
	//	functionName - the function at which the test is targeted
	//	testID - the unique id of the test
	//
	// Return Value: (int) the execution count of the test
	//*************************************************************************
	int HoloScriptApplication::GetScheduledTestExecutionCount(String * targetThreadID, String * category, String * functionName, String * testID)
	{
		SiString threadID;
		SiString cat;
		SiString func;
		SiString testIDName;
			
		Holodeck::StringConverter::GetInstance()->ConvertToSiString (targetThreadID, &threadID);
		Holodeck::StringConverter::GetInstance()->ConvertToSiString (category, &cat);
		Holodeck::StringConverter::GetInstance()->ConvertToSiString (functionName, &func);
		Holodeck::StringConverter::GetInstance()->ConvertToSiString (testID, &testIDName);

		if (settingsMgr != NULL)
			return settingsMgr->GetTestExecutionCount(threadID, cat, func, testIDName);

		return 0;
	}

	//*************************************************************************
	// Method:		CreateScheduledTest
	// Description: Creates a test to be performed on the next occurrence of
	//	a function call in the application under test if it meets the specified
	//	parameter values
	//
	// Parameters:
	//	targetThreadID - The thread(s) for which the test should be created
	//	category - The category of the function to create a test for
	//	functionName - The name of the function to create a test for
	//	returnValue - The value to return if the test matches
	//	errorCode - The error code to set if the test matches
	//	matchParams - The list of In parameters to match for the test to execute
	//	changeParams - The list of Out parameters that will change when the test executes
	//	testID - pointer to a unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::CreateScheduledTest(Array *targetThreadID, String *category, FaultFunction *functionTest)
	{
		const __wchar_t __pin *temp;
		FunctionTest *test = new FunctionTest();
		if (test == NULL)
			return false;

		// don't generate a new ID if we are modifying an existing test
		SiString *testIdStr = new SiString();

		// create a unique test id
		wchar_t *testIdStrW;
		if (!functionTest->TestID)
		{
			UUID uuid;
			UuidCreate(&uuid);
			UuidToStringW(&uuid, (wchar_t **)&testIdStrW);
			// set testID so that the new testID is returned to caller
			functionTest->TestID = testIdStrW;
			*testIdStr = testIdStrW;
			RpcStringFreeW((wchar_t **) &testIdStrW);
		}
		else
		{
			// use the existing
			testIdStrW = new wchar_t[functionTest->TestID->Length + 1];
			temp = PtrToStringChars(functionTest->TestID);
			wcsncpy(testIdStrW, temp, functionTest->TestID->Length);
			testIdStrW[functionTest->TestID->Length] = L'\0';
			testIdStr->operator =(testIdStrW);
			delete[] testIdStrW;
		}

		// check for a " - xxxx" in the error code
		int spacePos = -1;
		if (functionTest->ErrorCode)
		{
			int spacePos = functionTest->ErrorCode->IndexOf(" ");
			if (spacePos != -1)
				functionTest->ErrorCode = functionTest->ErrorCode->Substring(0, spacePos);
		}
		else
			functionTest->ErrorCode = "0";

		// fill in the test object
		test->SetTestID(*testIdStr);

		if (functionTest->ReturnValue)
		{
			wchar_t *returnValueStrW = new wchar_t[functionTest->ReturnValue->Length + 1];
			temp = PtrToStringChars(functionTest->ReturnValue);
			wcsncpy(returnValueStrW, temp, functionTest->ReturnValue->Length);
			returnValueStrW[functionTest->ReturnValue->Length] = L'\0';
			test->SetReturnValue(returnValueStrW);
		}
		else
			test->SetReturnValue("0");

		wchar_t *errorCodeStrW = new wchar_t[functionTest->ErrorCode->Length + 1];
		temp = PtrToStringChars(functionTest->ErrorCode);
		wcsncpy(errorCodeStrW, temp, functionTest->ErrorCode->Length);
		errorCodeStrW[functionTest->ErrorCode->Length] = L'\0';
		test->SetErrorCode(errorCodeStrW);

		if (functionTest->Exception)
		{
			wchar_t *exceptionStrW = new wchar_t[functionTest->Exception->Length + 1];
			temp = PtrToStringChars(functionTest->Exception);
			wcsncpy(exceptionStrW, temp, functionTest->Exception->Length);
			exceptionStrW[functionTest->Exception->Length] = L'\0';
			test->SetException(exceptionStrW);
		}

		test->SetAllocation((Replacement::AllocationType)functionTest->Allocation);
		test->SetPassThrough(functionTest->PassThrough);
		test->SetPassThroughReturnValue(functionTest->PassThroughReturnValue);
		test->SetPassThroughErrorCode(functionTest->PassThroughErrorCode);

		//bugbug - this is whacky... need to change to checkResource in FaultFunction.h
		if (functionTest->CheckResources->Count > 0)
		{
			Replacement::CheckResource checkResource;
			FaultFunction::CheckResource *checkResourceNew = dynamic_cast <FaultFunction::CheckResource*> (functionTest->CheckResources->Item[0]);
			checkResource.Exists = (Replacement::ExistsType) checkResourceNew->Exists;
			checkResource.ParamIndex = checkResourceNew->ParamIndex;
			test->SetCheckResource(checkResource);
		}

		if (functionTest->MatchParams)
		{
			IEnumerator *enumerator = functionTest->MatchParams->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
				if (!param)
					continue;

				//Check if any of the param test values == {Any}, if so, skip this param
				bool anyPresent = false;
				for (int cp=0; cp < param->TestCasesCount; cp++)
				{
					ParamTestCase * ptCase = param->TestCase(cp);
					if (ptCase->TestValue->CompareTo (IN_PARAM_MATCH_ANY_STRING) == 0)
					{
						anyPresent = true;
						break;
					}
				}
				if (anyPresent)
					continue;

				ParameterType type = (ParameterType)param->CompareAsType;
				int pos = param->ID;

				//Since no {Any} values were present for this parameter; start adding
				//the other match values to the test structure
				for (int paramMatch=0; paramMatch < param->TestCasesCount; paramMatch++)
				{
					ParamTestCase * ptCase = dynamic_cast <ParamTestCase *> (param->TestCase(paramMatch));
					void *value = NULL;
					int valueSize = 0;

					String *valueStr = ptCase->TestValue;

					ParameterOperatorType operatorType = EqualsType;
					if (ptCase->TestOperator->CompareTo(EQUAL_OPERATOR) == 0)
						operatorType = EqualsType;
					else if (ptCase->TestOperator->CompareTo(NOT_EQUAL_OPERATOR) == 0)
						operatorType = NotEqualToType;
					else if (ptCase->TestOperator->CompareTo(LESS_THAN_OPERATOR) == 0)
						operatorType = LessThanType;
					else if (ptCase->TestOperator->CompareTo(GREATER_THAN_OPERATOR) == 0)
						operatorType = GreaterThanType;
					else if (ptCase->TestOperator->CompareTo(LESS_EQUAL_OPERATOR) == 0)
						operatorType = LessThanOrEqualType;
					else if (ptCase->TestOperator->CompareTo(GREATER_EQUAL_OPERATOR) == 0)
						operatorType = GreaterThanOrEqualType;
					else if (ptCase->TestOperator->CompareTo(STARTS_WITH_OPERATOR) == 0)
						operatorType = StartsWithType;
					else if (ptCase->TestOperator->CompareTo(ENDS_WITH_OPERATOR) == 0)
						operatorType = EndsWithType;
					else if (ptCase->TestOperator->CompareTo(SUBSTRING_OPERATOR) == 0)
						operatorType = ContainsSubstringType;
					else if (ptCase->TestOperator->CompareTo(NOT_CONTAINS_OPERATOR) == 0)
						operatorType = NotContainsSubstringType;
					else if (ptCase->TestOperator->CompareTo(BINARY_CONTAINS_OPERATOR) == 0)
						operatorType = ContainsBinaryType;
					else if (ptCase->TestOperator->CompareTo(BINARY_NOT_CONTAINS_OPERATOR) == 0)
						operatorType = NotContainsBinaryType;
					else if (ptCase->TestOperator->CompareTo(EQUAL_RESOURCE_OPERATOR) == 0)
						operatorType = EqualsResourceType;
					else if (ptCase->TestOperator->CompareTo(NOT_EQUAL_RESOURCE_OPERATOR) == 0)
						operatorType = NotEqualToResourceType;
					else if (ptCase->TestOperator->CompareTo(CONTAINS_RESOURCE_OPERATOR) == 0)
						operatorType = ContainsResourceType;
					else if (ptCase->TestOperator->CompareTo(NOT_CONTAINS_RESOURCE_OPERATOR) == 0)
						operatorType = NotContainsResourceType;
					else if (ptCase->TestOperator->CompareTo(STARTS_WITH_RESOURCE_OPERATOR) == 0)
						operatorType = StartsWithResourceType;

					this->ConvertTestValueStringToValue(valueStr, &value, &valueSize, type);

					// add the parameter
					test->AddParameterToMatch(pos, paramMatch, value, valueSize, type, operatorType);

					// free the memory for the value
					if (value != NULL)
					{
						delete value;
						value = NULL;
						valueSize = 0;
					}
				}
			}
		}

		if (functionTest->ChangeParams)
		{
			IEnumerator *enumerator = functionTest->ChangeParams->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
				if (!param)
					continue;

				void *value = NULL;
				int valueSize = 0;

				ParameterType type = (ParameterType)param->CompareAsType;
				int pos = param->ID;
				String *valueStr = param->ChangeValue;

				// if the parameter doesn't need to change, don't add it.
				if (valueStr->CompareTo(OUT_PARAM_NO_CHANGE_STRING) == 0)
					continue;

				this->ConvertTestValueStringToValue(valueStr, &value, &valueSize, type);

				// add the parameter
				test->AddParameterToChange(pos, value, valueSize, type);
	
				// free the memory for the value
				if (value != NULL)
				{
					delete value;
					value = NULL;
					valueSize = 0;
				}
			}
		}
		
		//fill in the test firing values
		test->SetFiringOption (functionTest->FiringOption);
		test->SetPauseAppOnTestFire (functionTest->PauseAppAfterTestFire);
		SiArray <SiString> siafiringDetails;
		if (functionTest->FiringOption == 1)
		{
			String * detailValue = dynamic_cast <String *> (functionTest->FiringDetails);
			SiString sisDetailValue;
			
			Holodeck::StringConverter::GetInstance()->ConvertToSiString (detailValue, &sisDetailValue);
			siafiringDetails.Add (sisDetailValue);
		}
		else
		{
			ArrayList * array = this->ConvertDotNetNamesToHandlerNames (dynamic_cast <ArrayList *> (functionTest->FiringDetails));
			for (int i=0; i < array->Count; i++)
			{
				SiString sisValue;
				Holodeck::StringConverter::GetInstance()->ConvertToSiString (dynamic_cast <String *> (array->Item[i]), &sisValue);
				siafiringDetails.Add (sisValue);
			}
		}
		test->SetFiringDetails (siafiringDetails);

		bool retVal = true;

		if (scheduledTestWorkerRunning)
		{
			SiString* categoryStr = new SiString();
			SiString* functionStr = new SiString();
			Holodeck::StringConverter *converter = Holodeck::StringConverter::GetInstance();
			converter->ConvertToSiString(category, categoryStr);
			converter->ConvertToSiString(functionTest->Name, functionStr);
			
			ArrayList* targetThreads = new ArrayList();

			if (targetThreadID == NULL || (targetThreadID != NULL && targetThreadID->Length == 0) ||
				(targetThreadID != NULL && ((*dynamic_cast<__box UInt32*>(targetThreadID->Item[0])) == 0)))
				targetThreads->Add(new String(GLOBAL_THREAD_NAME));
			else
			{
				IEnumerator * threadEnum = targetThreadID->GetEnumerator();
				while (threadEnum->MoveNext())
					targetThreads->Add(threadEnum->Current->ToString());
			}

			ScheduledTestWorkItem* workItem = new ScheduledTestWorkItem;
			workItem->create = true;
			workItem->targetThreads = targetThreads;
			workItem->category = categoryStr;
			workItem->function = functionStr;
			workItem->test = test;
			workItem->testId = testIdStr;

			scheduledTestQueueMutex->WaitOne();
			scheduledTestWorkQueue->Add(workItem);
			scheduledTestQueueMutex->ReleaseMutex();
			
			scheduledTestQueueEvent->Signal();
		}
		else
		{
			// Clean up, since it will not be added to the queue
			delete testIdStr;
			delete test;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		DeleteScheduledTest
	// Description: Deletes a scheduled test
	//
	// Parameters:
	//	targetThreadID - the thread(s) from which the test should be deleted
	//	testID - pointer to a unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::DeleteScheduledTest(Array *targetThreadID, String *testID)
	{
		if (testID == NULL)
			return false;

		const __wchar_t __pin *testIdStr = PtrToStringChars(testID);
		bool retVal = true;
		if (scheduledTestWorkerRunning)
		{
			ArrayList* targetThreads = new ArrayList();

			scheduledTestQueueMutex->WaitOne();

			// If the test is still in the queue waiting to be created, remove it from the queue
			bool found = false;
			for (int i=0; i<scheduledTestWorkQueue->Count; i++)
			{
				ScheduledTestWorkItem* workItem = dynamic_cast<ScheduledTestWorkItem*>(scheduledTestWorkQueue->Item[i]);
				if (workItem->testId->CompareTo(testIdStr) == 0)
				{
					found = true;
					scheduledTestWorkQueue->RemoveAt(i);
					i--; // To maintain the same value of i in the next iteration
				}
			}

			// If the test was in the queue, it has already been deleted and the delete operation does not
			// need to be added to the queue
			if (found)
			{
				scheduledTestQueueMutex->ReleaseMutex();
				return retVal;
			}

			if (targetThreadID == NULL || (targetThreadID != NULL && targetThreadID->Length == 0) ||
				(targetThreadID != NULL && ((*dynamic_cast<__box UInt32*>(targetThreadID->Item[0])) == 0)))
				targetThreads->Add(new String(GLOBAL_THREAD_NAME));
			else
			{
				IEnumerator * threadEnum = targetThreadID->GetEnumerator();
				while (threadEnum->MoveNext())
					targetThreads->Add(threadEnum->Current->ToString());
			}

			ScheduledTestWorkItem* workItem = new ScheduledTestWorkItem;
			workItem->create = false;
			workItem->targetThreads = targetThreads;
			workItem->testId = new SiString(testIdStr);
			scheduledTestWorkQueue->Add(workItem);

			scheduledTestQueueMutex->ReleaseMutex();

			scheduledTestQueueEvent->Signal();
		}
		
		return retVal;
	}

	//*************************************************************************
	// Method:		CreateNetworkCorruptionFault
	// Description: Creates a network corruption fault
	//
	// Parameters:
	//	send - true if sent data should be corrupted
	//	recv - true if received data should be corrupted
	//	portList - array of network ports to corrupt
	//	searchRegExpr - regular expression to search for
	//	replacementStr - replacement string to replace each regular expression
	//		occurence with
	//	testID - pointer to a unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::CreateNetworkCorruptionFault(bool send, bool recv, Array* portList, String* searchRegExpr, String* replacementStr, String **testID)
	{
		// dont generate a new ID if we are modifying an existing test
		SiString *testIdStr = new SiString();

		// create a unique test id
		wchar_t *testIdStrW;
		if (*testID == NULL)
		{
			UUID uuid;
			UuidCreate(&uuid);
			UuidToStringW(&uuid, (wchar_t **)&testIdStrW);
			// set testID so that the new testID is returned to caller
			*testID = testIdStrW;
			*testIdStr = testIdStrW;
			RpcStringFreeW((wchar_t **) &testIdStrW);
		}
		else
		{
			// use the existing
			testIdStrW = new wchar_t[(*testID)->Length + 1];
			const __wchar_t __pin *temp = PtrToStringChars(*testID);
			wcsncpy(testIdStrW, temp, (*testID)->Length);
			testIdStrW[(*testID)->Length] = L'\0';
			testIdStr->operator =(testIdStrW);
		}

		const wchar_t __pin* wszSearchRegExpr = PtrToStringChars(searchRegExpr);
		const wchar_t __pin* wszReplacementStr = PtrToStringChars(replacementStr);
		unsigned char portMask[8192]; // 65536 bits, one for each possible port

		for (int i=0; i<8192; i++) // first clear port mask
			portMask[i] = 0;
		for (int i=0; i<portList->Length; i++)
		{
			int port = *dynamic_cast<__box int*>(portList->Item[i]);

			// Set the bit in the mask.  The mask is a bit array of 65536 bits, one
			// for each port, or 8192 bytes.  "port >> 3" divides the bit index by
			// 8, selecting which byte contains the bit we want to set.  "port & 7"
			// selects which bit index within the byte needs to be set.  The
			// "1 << (port & 7)" expression yields a value with only the selected
			// bit set.  We then OR this value with the existing value, which
			// sets the desired bit.
			portMask[port >> 3] |= 1 << (port & 7);
		}

		bool retVal = true;
		if (settingsMgr != NULL)
			retVal = settingsMgr->AddNetworkCorruptionFault(send, recv, portMask, wszSearchRegExpr, wszReplacementStr, *testIdStr);

		delete testIdStr;
		return retVal;
	}

	//*************************************************************************
	// Method:		DeleteNetworkCorruptionFault
	// Description: Deletes a network corruption fault
	//
	// Parameters:
	//	testID - pointer to a unique id that identifies this fault.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::DeleteNetworkCorruptionFault(String *testID)
	{
		const __wchar_t __pin *testIdStr = PtrToStringChars(testID);
		bool retVal = true;
		if (settingsMgr != NULL)
			retVal = settingsMgr->DeleteNetworkCorruptionFault(testIdStr);

		return retVal;
	}

	//*************************************************************************
	// Method:		CreateFileCorruptionFault
	// Description: Creates a file corruption fault
	//
	// Parameters:
	//	fileName - original file name
	//	alwaysRegen - regenerate corruption on each access
	//	searchRegExpr - regular expression to search for
	//	replacementStr - replacement string to replace each regular expression
	//		occurence with
	//	testID - pointer to a unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::CreateFileCorruptionFault(String* fileName, bool alwaysRegen, String* searchRegExpr, String* replacementStr, String **testID)
	{
		// dont generate a new ID if we are modifying an existing test
		SiString *testIdStr = new SiString();

		// create a unique test id
		wchar_t *testIdStrW;
		if (*testID == NULL)
		{
			UUID uuid;
			UuidCreate(&uuid);
			UuidToStringW(&uuid, (wchar_t **)&testIdStrW);
			// set testID so that the new testID is returned to caller
			*testID = testIdStrW;
			*testIdStr = testIdStrW;
			RpcStringFreeW((wchar_t **) &testIdStrW);
		}
		else
		{
			// use the existing
			testIdStrW = new wchar_t[(*testID)->Length + 1];
			const __wchar_t __pin *temp = PtrToStringChars(*testID);
			wcsncpy(testIdStrW, temp, (*testID)->Length);
			testIdStrW[(*testID)->Length] = L'\0';
			testIdStr->operator =(testIdStrW);
		}

		const wchar_t __pin* wszOrigFile = PtrToStringChars(fileName);
		const wchar_t __pin* wszSearchRegExpr = PtrToStringChars(searchRegExpr);
		const wchar_t __pin* wszReplacementStr = PtrToStringChars(replacementStr);

		bool retVal = true;
		if (settingsMgr != NULL)
		{
			retVal = settingsMgr->AddFileCorruptionFault(wszOrigFile, alwaysRegen, wszSearchRegExpr, wszReplacementStr, *testIdStr);
			settingsMgr->ClearLastFileCorruption(wszOrigFile);
		}

		delete testIdStr;
		return retVal;
	}

	//*************************************************************************
	// Method:		RegenerateFileCorruptionFault
	// Description: Forces regeneration of a file corruption at the next
	//		access to the original file
	//
	// Parameters:
	//	fileName - original file name
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::RegenerateFileCorruptionFault(String *fileName)
	{
		const wchar_t __pin* wszOrigFile = PtrToStringChars(fileName);
		wchar_t wszFullPath[MAX_PATH];

		if (!GetFullPathNameW(wszOrigFile, MAX_PATH, wszFullPath, NULL))
			return false;
		if (!GetLongPathNameW(wszFullPath, wszFullPath, MAX_PATH))
			return false;
		wcslwr(wszFullPath);

		return settingsMgr->ClearLastFileCorruption(wszFullPath);
	}

	//*************************************************************************
	// Method:		SetLastFileCorruption
	// Description: Sets the value of the last corrupted file; so that file corruption
	//				code may repro file corruption if required.
	//
	// Parameters:
	//	fileName - original file name
	//	corruptedFile - the name and path of the corrupted file
	//	changesXmlFile - the name and path of the changes XML file
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::SetLastFileCorruption(String *fileName, String *corruptedFile, String *changesXmlFile)
	{
		bool retVal = false;
		if (settingsMgr != NULL)
		{
			const wchar_t __pin* wszfileName = PtrToStringChars(fileName);
			const wchar_t __pin* wszcorruptedFile = PtrToStringChars(corruptedFile);
			const wchar_t __pin* wszchangesXmlFile = PtrToStringChars(changesXmlFile);

			retVal = settingsMgr->SetLastFileCorruption (wszfileName, wszcorruptedFile, wszchangesXmlFile);
		}

		return retVal;

	}

	//*************************************************************************
	// Method:		DeleteFileCorruptionFault
	// Description: Deletes a file corruption fault
	//
	// Parameters:
	//	testID - pointer to a unique id that identifies this fault.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool HoloScriptApplication::DeleteFileCorruptionFault(String *testID)
	{
		const __wchar_t __pin *testIdStr = PtrToStringChars(testID);
		bool retVal = true;
		if (settingsMgr != NULL)
			retVal = settingsMgr->DeleteFileCorruptionFault(testIdStr);
		
		return retVal;
	}

	//*************************************************************************
	// Method:		GetApplicationThreadList
	// Description: Gets the list of threads used by the process
	//
	// Parameters:
	//	threadIDList - stores the ids of the threads, make this param NULL
	//				   to get array size information from the return value
	//
	// Return Value: the number of threads in the current process.
	//*************************************************************************
	int HoloScriptApplication::GetApplicationThreadList(DWORD * threadIDList)
	{
		return heat->getThreadsInCurrentProcess (threadIDList);
	}

	//*************************************************************************
	// Method:		ConvertTestValueStringToValue
	// Description: converts a value string to a value depending on the type
	//
	// Parameters:
	//	valueStr - the value to convert
	//	value - the resulting value
	//	valueSize - the resulting size of the value in bytes
	//	type - the parameter type to use to convert
	//
	// Return Value: the number of threads in the current process.
	//*************************************************************************
	void HoloScriptApplication::ConvertTestValueStringToValue(String *valueStr, void **value, int *valueSize, ParameterType type)
	{
		// make the value from the value string based on the parameter type
		switch(type)
		{
			case NullType:
			{
				break;
			}
			case StringType:
			case OutStringType:
			{
				if (valueStr)
				{
					int strLength = valueStr->Length;
					*value = new char[strLength + 1];
					const __wchar_t __pin *chars = PtrToStringChars(valueStr);
					wcstombs((char *)*value, chars, strLength);
					((char *)*value)[strLength] = '\0';
					*valueSize = sizeof(char) * (strLength + 1);
				}
				else
				{
					*value = NULL;
					*valueSize = 0;
				}
				break;
			}
			case IIDRefType:
			case WideStringType:
			case OutWideStringType:
			{
				if (valueStr)
				{
					int strLength = valueStr->Length;
					*value = new wchar_t[strLength + 1];
					const __wchar_t __pin *chars = PtrToStringChars(valueStr);
					wmemcpy((wchar_t *)*value, chars, strLength);
					((wchar_t *)*value)[strLength] = L'\0';
					*valueSize = sizeof(wchar_t) * (strLength + 1);
				}
				else
				{
					*value = NULL;
					*valueSize = 0;
				}
				break;
			}
			case BooleanType:
			{
				*value = new bool;

				if (valueStr->Equals(IN_PARAM_MATCH_TRUE_STRING))
					*((bool *)*value) = true;
				else if (valueStr->Equals(IN_PARAM_MATCH_FALSE_STRING))
					*((bool *)*value) = false;
				else
					*((bool *)*value) = (valueStr->CompareTo(LOG_TRUE_DISPLAY_STRING) == 0);

				*valueSize = sizeof(bool);
				break;
			}
			case IntegerType:
			{
				*value = new int;
				const __wchar_t __pin *chars = PtrToStringChars(valueStr);
				*((int *)*value) = _wtoi(chars);
				*valueSize = sizeof(int);
				break;
			}
			case UnsignedLongType:
			{
				*value = new unsigned long;
				const __wchar_t __pin *chars = PtrToStringChars(valueStr);
				wchar_t *stopChars;
				*((unsigned long *)*value) = wcstoul(chars, &stopChars, 10);
				*valueSize = sizeof(unsigned long);
				break;
			}
			case RealType:
			{
				*value = new double;
				const __wchar_t __pin *chars = PtrToStringChars(valueStr);
				*((double *)*value) = _wtof(chars);
				*valueSize = sizeof(double);
				break;
			}
			case PointerType:
			{
				*value = new unsigned long;
				
				if (valueStr->Equals(IN_PARAM_MATCH_NULL_STRING))
					*((unsigned long *)*value) = 0;
				else
				{
					const __wchar_t __pin *chars = PtrToStringChars(valueStr);
					wchar_t *stopChars;
					*((unsigned long *)*value) = wcstoul(chars, &stopChars, 10);
				}

				*valueSize = sizeof(unsigned long);
				break;
			}
			case Integer64Type:
			{
				*value = new __int64;
				const __wchar_t __pin *chars = PtrToStringChars(valueStr);
				*((__int64 *)*value) = _wtoi64(chars);
				*valueSize = sizeof(__int64);
				break;
			}
			case IgnoreType:
			{
				*value = NULL;
				*valueSize = 0;
				break;
			}
			default: // treat as an integer for now, might need to change
			{
				*value = new int;
				const __wchar_t __pin *chars = PtrToStringChars(valueStr);
				*((int *)*value) = _wtoi(chars);
				*valueSize = sizeof(int);
				break;
			}
		}
	}

	//*************************************************************************
	// Method:		InterceptFunction
	// Description: Adds function details to the interceptionList array
	//				Used when disabledlist is not passed in Launch or Attach
	//
	// Parameters:
	//	functionName - name of the function
	//  redirectedFunctionName - replacement function
	//	redirectedDllName - library in which the replacement function resides
	//
	// Return Value: true if successful
	//*************************************************************************
	bool HoloScriptApplication::InterceptFunction(String * functionName, String * redirectedFunctionName, String * redirectedDllName)
	{
		InterceptionInfo * newFunction = new InterceptionInfo();
		newFunction->functionName = functionName;
		newFunction->redirectedDllName = redirectedDllName;
		newFunction->redirectedFunctionName = redirectedFunctionName;

		this->interceptionList->Add (newFunction);
		return true;		
	}

	//*************************************************************************
	// Method:		GetFilteredFunctionList
	// Description: Gets the complimentary of the interceptionList. Used in by
	//				LaunchApplication and AttachApplication when disabled list
	//				is not provided.
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList of functions to disable
	//*************************************************************************
	ArrayList * HoloScriptApplication::GetFilteredFunctionList()
	{
		ArrayList * filterfuncs = new ArrayList();
		IEnumerator *w32enumerator = db->Functions->GetEnumerator();
		InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		IEnumerator *dotNetenumerator = dotNetDB->get_Functions()->GetEnumerator();

		Hashtable* interceptionHash = new Hashtable();
		for (int i=0; (i < this->interceptionList->Count); i++)
		{
			InterceptionInfo * interceptInfo = dynamic_cast <InterceptionInfo *> (this->interceptionList->Item[i]);
			String *name = interceptInfo->functionName->Trim()->ToLower();
			if (!interceptionHash->Contains(name))
				interceptionHash->Add(name, __box(true));
		}

		// check from the Win32 DB
		while (w32enumerator->MoveNext())
		{
			InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(w32enumerator->Current);
			bool filtered = !interceptionHash->ContainsKey(func->Name->Trim()->ToLower());

			if (filtered)
				filterfuncs->Add (func->Name);
		}

		// check from the .NET DB
		while (dotNetenumerator->MoveNext())
		{
			InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(dotNetenumerator->Current);
			bool filtered = !interceptionHash->ContainsKey(func->Name->Trim()->ToLower());

			if (filtered)
				filterfuncs->Add (func->Name);
		}

		return filterfuncs;
	}

	//*************************************************************************
	// Method:		onChildProcessCreated
	// Description: receives notification that a child process has been created
	//
	// Parameters:
	//	newProcess - true if this is a new process, false if a new thread
	//	id - the id of the new process of thread
	//	resume - true if the new aut should be resumed, false if it should stay
	//		suspended
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::onChildProcessCreated(bool newProcess, unsigned long id, bool resume)
	{
		UInt32 currentPid;

		this->GetApplicationProcessID(currentPid);
		if (newProcess)
		{
			if (this->ProcessChaining)
			{
				if (OnAttachToChildProcess)
					OnAttachToChildProcess->Invoke(currentPid, id, resume);
				else
				{
					HoloScriptApplication *newHoloScriptApp = new HoloScriptApplication();
					newHoloScriptApp->PauseOnStart = !resume;

					if (this->OnInitializeAppForProcessChaining)
						this->OnInitializeAppForProcessChaining->Invoke(this, newHoloScriptApp);

					if (newHoloScriptApp->AttachApplication(id, true, newHoloScriptApp->FilteredFunctionList, true))
					{
						if (this->OnProcessChainingAttachComplete)
							this->OnProcessChainingAttachComplete->Invoke(this, newHoloScriptApp);

						if (!newHoloScriptApp->PauseOnStart)
						{
							newHoloScriptApp->ResumeApplication();
						}
					}
				}
			}
			else
			{
				// if process chaining was off for this process the process would never get resumed
				// without the code below making the user think that the app never started
				HoloScriptApplication *newHoloScriptApp = new HoloScriptApplication();
				newHoloScriptApp->PauseOnStart = !resume;

				if (newHoloScriptApp->AttachApplication(id, true, newHoloScriptApp->FilteredFunctionList, true))
				{
					if (!newHoloScriptApp->PauseOnStart)
					{
						newHoloScriptApp->ResumeApplication();
					}
				}
			}
		}
		else
		{
			if (this->OnThreadCreated)
				this->OnThreadCreated->Invoke(currentPid, id);
		}
	}

	//*************************************************************************
	// Method:		StartProcessWatching
	// Description: sets up the process watching events and starts the thread.
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::StartProcessWatching()
	{
		//set up process listening events
		char eventName[64];
		char pid[28];
		if (processTerminatedEvent == NULL)
		{
			strcpy(eventName, "ProcessTerminatedEvent");
			itoa(heat->getProcessId(), pid, 10);
			strcat(eventName, pid);
			processTerminatedEvent = new Event();
			processTerminatedEvent->Create(eventName);
		}

		if (processPausedEvent == NULL)
		{
			strcpy(eventName, "ProcessPausedEvent");
			itoa(heat->getProcessId(), pid, 10);
			strcat(eventName, pid);
			processPausedEvent = new Event();
			processPausedEvent->Create(eventName);
		}

		if (processResumedEvent == NULL)
		{
			strcpy(eventName, "ProcessResumedEvent");
			itoa(heat->getProcessId(), pid, 10);
			strcat(eventName, pid);
			processResumedEvent = new Event();
			processResumedEvent->Create(eventName);
		}

		ThreadStart *processWatchThreadStart = new ThreadStart(this, ProcessWatchThreadFunction);
		processWatchThread = new Thread(processWatchThreadStart);
		processWatchThread->IsBackground = true;
		processWatchThreadRunning = true;
		processWatchThread->Start();
	}

	//*************************************************************************
	// Method:		StopProcessWatching
	// Description: stops the process watching thread.
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::StopProcessWatching()
	{
		if (processWatchThreadRunning)
		{
			processWatchThreadRunning = false;
			processWatchThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
			processWatchThread = NULL;
		}

		//remove process listening events
		if (processTerminatedEvent != NULL)
		{
			delete processTerminatedEvent;
			processTerminatedEvent = NULL;
		}

		if (processPausedEvent != NULL)
		{
			delete processPausedEvent;
			processPausedEvent = NULL;
		}

		if (processResumedEvent != NULL)
		{
			delete processResumedEvent;
			processResumedEvent = NULL;
		}
	}

	//*************************************************************************
	// Method:		ProcessWatchThreadFunction
	// Description: watches for heat events that tell of process status changes
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::ProcessWatchThreadFunction()
	{
		unsigned int retVal;
		unsigned int pid = heat->getProcessId();

		Event* eventArray[3];
		eventArray[0] = processTerminatedEvent;
		eventArray[1] = processPausedEvent;
		eventArray[2] = processResumedEvent;

		while (processWatchThreadRunning)
		{
			retVal = Event::AcquireMultiple(eventArray, 3, false, PROCESS_WATCH_WAIT_TIME);

			if (retVal == WAIT_FAILED)
				return;

			if (retVal == WAIT_OBJECT_0) // processTerminatedEvent
			{
				if (OnHeatProcessTerminated)
					OnHeatProcessTerminated->Invoke(pid);
			}

			if (retVal == (WAIT_OBJECT_0 + 1)) // processPausedEvent
			{
				if (OnHeatProcessPaused)
					OnHeatProcessPaused->Invoke(pid);
			}

			if (retVal == (WAIT_OBJECT_0 + 2)) // processResumedEvent
			{
				if (OnHeatProcessResumed)
					OnHeatProcessResumed->Invoke(pid);
			}
		}
	}

	//*************************************************************************
	// Method:		set_AttachDebugger
	// Description: Sets the AttachDebugger property.  If it is true, a
	//				debugger will be attached and the OnExceptionThrown
	//				event will be triggered.
	//
	// Parameters:
	//	value - New value of AttachDebugger property
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::set_AttachDebugger(bool value)
	{
		attachDebugger = value;
		if (attachDebugger)
			AttachDebuggerToProcess();
		else if (debugger)
			DetachDebuggerFromProcess();
	}

	//*************************************************************************
	// Method:		set_LogFirstChance
	// Description: Sets the LogFirstChance property.  If it is true, the
	//				debugger will log first chance exceptions, otherwise it
	//				will ignore them.
	//
	// Parameters:
	//	value - New value of LogFirstChance property
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::set_LogFirstChance(bool value)
	{
		logFirstChance = value;
		if (debugger)
			debugger->LogFirstChance = value;
	}

	//*************************************************************************
	// Method:		ExceptionThrownHandler
	// Description: Called when an exception is caught by the debugger
	//
	// Parameters:
	//	processId - process id that triggered the exception
	//  info - exception information
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::ExceptionThrownHandler(unsigned int processId, ExceptionInfo info)
	{
		// Pass the exception information to the user
		if (OnExceptionThrown)
			OnExceptionThrown->Invoke(processId, info);
	}

	//*************************************************************************
	// Method:		get_ProjectFolder
	// Description: Gets the path to the folder where the project file is stored
	//
	// Parameters:
	//	None
	//
	// Return Value: Project folder
	//*************************************************************************
	String* HoloScriptApplication::get_ProjectFolder()
	{
		SiString folder;
		settingsMgr->GetProjectFolder(folder);
		return new String((wchar_t*)folder);
	}

	//*************************************************************************
	// Method:		get_ProjectFolder
	// Description: Gets the title of the project (no path, no extension)
	//
	// Parameters:
	//	None
	//
	// Return Value: Project title
	//*************************************************************************
	String* HoloScriptApplication::get_ProjectTitle()
	{
		SiString title;
		settingsMgr->GetProjectTitle(title);
		return new String((wchar_t*)title);
	}

	//*************************************************************************
	// Method:		AttachDebuggerToProcess
	// Description: Attaches a debugger to the process to catch exceptions
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::AttachDebuggerToProcess()
	{
		if (debugger != NULL)
			return; // already attached
		if (!heat->isReady())
			return; // not attached to an app yet

		debugger = new HoloScriptDebugger;
		debugger->LogFirstChance = logFirstChance;
		debugger->Attach(this);

		// Pass exceptions from the debugger to HoloScript so we can notify the user
		debugger->OnExceptionThrown = new ExceptionThrownDelegate(this, ExceptionThrownHandler);
	}

	//*************************************************************************
	// Method:		DetachDebuggerFromProcess
	// Description: Detaches the debugger from the process if it was attached
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::DetachDebuggerFromProcess()
	{
		if (debugger)
		{
			debugger->Detach();
			debugger = NULL;
		}
	}

	//*************************************************************************
	// Method:		AddExternalDotNetLibrary
	// Description: Adds the specified library to the list of external .NET libs
	//				This list is passed to Kernel32ReplacementLibrary.dll via the
	//				registry.
	//
	// Parameters:
	//	externalLibrary	- the external library
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::AddExternalDotNetLibrary(String * externalLibrary)
	{
		if (!externalDotNetInterceptionLibs)
			externalDotNetInterceptionLibs = new SiArray<SiString>();

		if (!converter)
			converter = Holodeck::StringConverter::GetInstance();

		SiString extLib;
		converter->ConvertToSiString(externalLibrary, &extLib);
		externalDotNetInterceptionLibs->Add (extLib);
	}

	//*************************************************************************
	// Method:		ConvertDotNetNamesToHandlerNames
	// Description: Converts class names for .NET functions from real class names
	//				to heat assigned replacement class names.
	//				Example: System.IO.File.CreateText becomes HandlerClass22.CreateText
	//				
	// Parameters:
	//	realNamesArray - the original array
	//
	// Return Value: an array with hot class names
	//*************************************************************************
	ArrayList * HoloScriptApplication::ConvertDotNetNamesToHandlerNames(ArrayList * realNamesArray)
	{
		//If list is empty; return empty list
		if (realNamesArray == NULL || realNamesArray->Count == 0)
			return realNamesArray;
		
		//Load Function Database if not already loaded
		db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);

		//Create a new names array
		ArrayList * newNamesArray = new ArrayList (realNamesArray->Count);

		//Convert the class names of .NET functions
		for (int i=0; i < realNamesArray->Count; i++)
		{
			InterceptedFunction * func = db->GetFunctionByName (dynamic_cast <String *> (realNamesArray->Item[i]));

			//If NULL then function was native or internal to AUT; else it was DotNet
			if (func != NULL)
			{
				String *realName = dynamic_cast <String*> (realNamesArray->Item[i]);
				String *newName;

				int tmpIndex = realName->IndexOf('(');
				if ( (tmpIndex != -1) && (realName->Length > tmpIndex) )
					newName = realName->Substring(0, tmpIndex);

				newNamesArray->Add(newName);
			}
			else
				newNamesArray->Add (dynamic_cast <String *> (realNamesArray->Item[i]));

		}

		//return the converted array
		return newNamesArray;
	}

	//*************************************************************************
	// Method:		ScheduledTestWorkerThreadFunction
	// Description: Thread that will perform the actions in the scheduled
	//				test work queue.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::ScheduledTestWorkerThreadFunction()
	{
		while (scheduledTestWorkerRunning)
		{
			scheduledTestQueueEvent->Acquire(INFINITE);

			while (true)
			{
				scheduledTestQueueMutex->WaitOne();
				
				if (scheduledTestWorkQueue->Count == 0)
				{
					// No items left, go back into the wait state
					scheduledTestQueueMutex->ReleaseMutex();
					break;
				}

				// Pop off the front of the queue
				ScheduledTestWorkItem* workItem = dynamic_cast<ScheduledTestWorkItem*>(scheduledTestWorkQueue->Item[0]);
				scheduledTestWorkQueue->RemoveAt(0);

				scheduledTestQueueMutex->ReleaseMutex();

				// Perform the queued action
				if (workItem->create)
				{
					if (settingsMgr != NULL)
					{
						// Create a function test
						for (int i=0; i<workItem->targetThreads->Count; i++)
						{
							SiString tID;
							converter->ConvertToSiString(dynamic_cast<String*>(workItem->targetThreads->Item[i]), &tID);
							settingsMgr->AddFunctionTest(tID, *workItem->category, *workItem->function,
								workItem->test, workItem->testId);
						}
					}

					// Clean up
					delete workItem->category;
					delete workItem->function;
					delete workItem->test;
					delete workItem->testId;
				}
				else
				{
					if (settingsMgr != NULL)
					{
						// Delete a function test
						for (int i=0; i<workItem->targetThreads->Count; i++)
						{
							SiString tID;
							converter->ConvertToSiString(dynamic_cast<String*>(workItem->targetThreads->Item[i]), &tID);
							settingsMgr->DeleteFunctionTest(tID, *workItem->testId);
						}
					}

					// Clean up
					delete workItem->testId;
				}
			}
			//pulse the complete event
			scheduledTestQueueCompleteEvent->Signal();
		}
	}

	//*************************************************************************
	// Method:		LockOnLogGenerated
	// Description: Prevents other threads from generating log generated
	//				events.  Allows traversal of the current set of logs with
	//				the guarantee that no logs can be missed.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::LockOnLogGenerated()
	{
		if (logReader)
			logReader->LogEventMutex->Acquire();
	}

	//*************************************************************************
	// Method:		UnlockOnLogGenerated
	// Description: Allows other threads to once again generate log generated
	//				events.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::UnlockOnLogGenerated()
	{
		if (logReader)
			logReader->LogEventMutex->Release();
	}

	//*************************************************************************
	// Method:		EnumerateSymbols
	// Description: Enumerates the symbols in the process specified and sets the
	//				processSymbols member
	//
	// Parameters:
	//	hProcess - handle to the process for which to get symbols (process must
	//			   NOT be suspended)
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptApplication::EnumerateSymbols(HANDLE hProcess)
	{
		const __wchar_t __pin *appPath = PtrToStringChars(this->ApplicationPath);
		String * moduleName = String::Concat (Path::GetFileNameWithoutExtension(this->ApplicationPath), S"!");
		
		PEFile * pefile = new PEFile();
		pefile->load (appPath, true);
		
		if (pefile->isManagedExecutable())
		{
			SiArray <CLRTypeName> types = pefile->enumerateTypes();
			
			if (types.GetLength() > 0)
			{
				ArrayList *dotnetProcSymbolList = new ArrayList();
			
				for (int i=0; i < types.GetLength(); i++)
				{
					CLRTypeName typeName = types[i];
					SiArray <CLRMemberDescription> members = pefile->enumerateMethods (typeName);
					for (int j=0; j < members.GetLength(); j++)
					{
						CLRMemberDescription member = members[j];
						String * symbolName = String::Concat (new String ((wchar_t *)typeName.sName), S".", new String((wchar_t*)member.sName));
						dotnetProcSymbolList->Add (String::Concat (moduleName, S" ", symbolName->Replace ("..", ".")));
					}
				}

				this->processSymbolsArray = dotnetProcSymbolList->ToArray();
			}
			else
				this->processSymbolsArray = NULL;
		}
		else
		{
			//Save current working directory
			String * savedworkDir = IO::Directory::GetCurrentDirectory();
			//Set working dir to that of app exe path
			IO::Directory::SetCurrentDirectory (Path::GetDirectoryName(this->ApplicationPath));

			String *installPath;
			SiString dbgHelpDir;
			RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));

			this->triedGettingSymbols = true;
			char* szModuleName = (char*)(void*)Marshal::StringToHGlobalAnsi(moduleName);
			Symbols * sym = new Symbols();
			if (installPath)
			{
				Holodeck::StringConverter::GetInstance()->ConvertToSiString(installPath, &dbgHelpDir);
				sym->SetPathToDbgHelpDll(dbgHelpDir);
			}

			SiArray<SiString> * symbols = sym->GetSymbolsForProcess (hProcess, szModuleName);
			Marshal::FreeHGlobal(szModuleName);
			
			//Restore working directory
			IO::Directory::SetCurrentDirectory (savedworkDir);
			
			if (symbols != NULL)
			{
				ArrayList *processSymbolList = new ArrayList(symbols->GetLength());
				
				for (int i=0 ; i< symbols->GetLength(); i++)
				{
					SiString symbol = (*symbols)[i];
					String * symbolName = new String ((wchar_t*) symbol);
					processSymbolList->Add(symbolName->Replace ("::", "."));
				}
				
				this->processSymbolsArray = processSymbolList->ToArray();
			}
			else
				this->processSymbolsArray = NULL;
		}
	}

	//*************************************************************************
	// Method:		GetUUID
	// Description: Returns a UUID
	//
	// Parameters:
	//	None
	//
	// Return Value: Returns a UUID string
	//*************************************************************************
	String* HoloScriptApplication::GetUUID()
	{
		String * uuidStr = "";
		wchar_t *testIdStrW;
		
		UUID uuid;
		UuidCreate(&uuid);
		UuidToStringW(&uuid, (wchar_t **)&testIdStrW);
		uuidStr = testIdStrW;
		RpcStringFreeW((wchar_t **) &testIdStrW);

		return uuidStr;
	}

	void HoloScriptApplication::TestPauseWatchThreadFunction()
	{
		while (testPauseWatchThreadRunning)
		{
			if ((settingsMgr == NULL) || (heat == NULL))
				break;

			if (settingsMgr->WaitForTestPausedEvent(TEST_PAUSE_WATCH_WAIT_TIME))
			{
				// Notify the UI that the AUT is now paused
				if (OnHeatProcessPaused)
					OnHeatProcessPaused->Invoke(heat->getProcessId());
			}
		}
	}
}