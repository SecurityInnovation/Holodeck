//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HoloScriptApplication.h
//
// DESCRIPTION: Contains definition for the class HoloScriptApplication
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "CHeatApp.h"
#include "SettingsManager.h"
#include "LogReceiver.h"
#include "LogItem.h"
#include "NotifiableLogReceiver.h"
#include "Mutex.h"
#include "Event.h"
#include "ChildProcessListener.h"
#include "LogReader.h"
#include "defines.h"
#include "Debugger.h"
#include "SiArray.h"
#include "StringConverter.h"
#include <dbghelp.h>
#include <psapi.h>
#include "Symbols.h"
#include "PEFile.h"

#using <mscorlib.dll>
#using <functiondatabase.dll>
#using <System.dll>
#using <System.ServiceProcess.dll>
#using <System.Management.dll>
#using <System.Configuration.Install.dll>

using namespace System;
using namespace System::Collections;
using namespace System::IO;
using namespace Microsoft::Win32;
using namespace FunctionDatabase;
using namespace Replacement;
using namespace System::Diagnostics;
using namespace System::ServiceProcess;
using namespace System::Management;
using namespace System::Configuration::Install;
using namespace SiUtils;
using namespace System::Runtime::InteropServices;

namespace HoloScript
{
	public __delegate void InitializeAppForProcessChainingDelegate(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp);
	public __delegate void ProcessChainingAttachCompleteDelegate(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp);
	public __delegate void AttachToChildProcessDelegate(unsigned long parentPid, unsigned long pid, bool resumeApp);
	public __delegate void ThreadCreatedDelegate(unsigned int pid, unsigned int tid);
	public __delegate void HeatProcessTerminatedDelegate(unsigned int processID);
	public __delegate void HeatProcessPausedDelegate(unsigned int processID);
	public __delegate void HeatProcessResumedDelegate(unsigned int processID);
	public __delegate void CantCreateLogFileDelegate();

	public __gc struct ScheduledTestWorkItem
	{
		bool create;
		SiString* testId;
		ArrayList* targetThreads;
		SiString* category;
		SiString* function;
		FunctionTest* test;
	};

	public __gc struct InterceptionInfo
	{
	public:
		InterceptionInfo()
		{
			functionName = "";
			redirectedFunctionName = "";
			redirectedDllName = "";
		}
		String * functionName;
		String * redirectedFunctionName;
		String * redirectedDllName;
	};

	public __gc class HoloScriptApplication
	{
	private:
		CHeatApp *heat;						// the HEAT object used to inject the aut
		SettingsManager *settingsMgr;		// gets and sets settings for the aut to use
		LogReceiver *logReceiver;			// receives log notification
		ChildProcessListener *childProcListener;	// receives child process creation notification
		LogReader *logReader;				// reads the log files
		bool isAlreadyIntercepting;			// true if we are already injected into the process
		bool appNeedsToBeTerminated;		// true unless the process was running before we inject it
		bool processChaining;				// true if process chaining is enabled, false otherwise
		bool inheritSettings;				// true if child process inherits parents settings, false otherwise
		bool isChainedProcess;				// true if process is a child of another intercepted process
		bool isService;						// true if launched or attached to a service
		bool threadRunning;					// while true, the can't create log file thread will run
		bool attachDebugger;				// true if debugger should be attached
		bool shouldRestart;					// false if aut should not be restarted on project restart
		bool functionLoggingUpdate;			// true during batch update to function logging options
		bool firstResume;					// true if application is being resumed for the first time

		Holodeck::StringConverter * converter;
		Thread *cantCreateLogFileListeningThread;
		SiArray <SiString> * externalDotNetInterceptionLibs;
		ArrayList *filteredFunctionList;	// list of functions to filter - null if attached to
		ArrayList *interceptionList;		// list of function to intercept (used for LibraryExporter functions)
		String *applicationToLaunch;		// path to application launched
		String *commandLineParams;			// command line parameters passed to the launched app - null if attached to
		String *serviceName;				// the name of the service we launch/attach
		bool pauseOnStart;					// retains pause setting from launch - false if attached to
		InterceptedFunctionDB *db;			// Intercepted Function Database
		bool appAttachedTo;					// True if app was attached to
		bool savedLog;						// True if working with a saved log, otherwise false
		String *projectPath;				// full path for current project including name
		ServiceController *pendingServiceToStart;
		Event *cantCreateLogFileEvent;

		ArrayList *scheduledTestWorkQueue;	// Contains scheduled tests queued for creation or deletion
		Event *scheduledTestQueueEvent;		// Triggered when a work item is added to the scheduled test queue
		Event *scheduledTestQueueCompleteEvent;	// Triggered when the scheduled test queue is complete with a batch of work items
		static System::Threading::Mutex* scheduledTestQueueMutex = new System::Threading::Mutex(false);
		Thread *scheduledTestWorkerThread;	// Worker thread for the scheduled test queue
		bool scheduledTestWorkerRunning;

		Event *processTerminatedEvent;
		Event *processPausedEvent;
		Event *processResumedEvent;
		static Event *waitForLaunchedServiceEvent = NULL;
		static Event *readyToContinueLaunchEvent = NULL;
		static Event *waitForServiceStopEvent = NULL;
		static String *serviceToLaunchInAThread = NULL;
		static int launchedServiceProcessId = 0;

		Thread *processWatchThread;
		bool processWatchThreadRunning;
		Thread *testPauseWatchThread;
		bool testPauseWatchThreadRunning;

		System::Diagnostics::Process *process;

		HoloScriptDebugger* debugger;
		bool logFirstChance;

		void CleanUp();
		
		void StartProcessWatching();
		void StopProcessWatching();
		void ProcessWatchThreadFunction();

		void onChildProcessCreated(bool newProcess, unsigned long id, bool resume);
		
		bool InterceptFunctionsFromDatabase();
		bool InterceptFunctionsFromInterceptionList();
		ArrayList *GetFilteredFunctionList();

		LogItem *GetLogEntryFromFile(ReceivedLogInfo *info, int index, unsigned long &filePosition);
		void ConvertTestValueStringToValue(String *valueStr, void **value, int *valueSize, ParameterType type);

		void CantCreateLogFileThreadFunction();
		void StartServiceThreadFunction();
		void StopServiceThreadFunction();
		void onAttachToLaunchedService(unsigned long parentPid, unsigned long pid, bool resumeApp);
		void AttachToServicesExe();
		bool SetApplicationName(unsigned long pid);
		void AttachDebuggerToProcess();
		void DetachDebuggerFromProcess();
		
		ArrayList *ConvertDotNetNamesToHandlerNames(ArrayList * realNamesArray);
		void ScheduledTestWorkerThreadFunction();

		void TestPauseWatchThreadFunction();
		
		//Members which get symbols for the aut
		void EnumerateSymbols(HANDLE hProcess);
		Array *processSymbolsArray;
		bool triedGettingSymbols;

	public:
		HoloScriptApplication();
		~HoloScriptApplication();

		AttachToChildProcessDelegate *OnAttachToChildProcess;
		InitializeAppForProcessChainingDelegate *OnInitializeAppForProcessChaining;
		ProcessChainingAttachCompleteDelegate *OnProcessChainingAttachComplete;
		ThreadCreatedDelegate *OnThreadCreated;
		CantCreateLogFileDelegate *OnCantCreateLogFile;
		ExceptionThrownDelegate* OnExceptionThrown;

		HeatProcessTerminatedDelegate	*OnHeatProcessTerminated;
		HeatProcessPausedDelegate		*OnHeatProcessPaused;
		HeatProcessResumedDelegate		*OnHeatProcessResumed;

		static int GetProcessIdForProcessName(String *procNameToFind);

		__property LogsGeneratedDelegate *get_OnLogGenerated() { return logReader->OnLogGenerated; }

		__property unsigned long get_LogEntryCount() { return (logReader ? logReader->LogEntryCount : 0); }

		__property Array* get_ProcessSymbols() { return processSymbolsArray; }
		__property bool get_ShouldRestartWithProject() { return shouldRestart; }
		__property void set_ShouldRestartWithProject(bool value) {shouldRestart = value; }
		__property bool get_ProcessChaining() { return processChaining; }
		__property void set_ProcessChaining(bool value) { processChaining = value; }
		__property bool get_InheritSettings() { return inheritSettings; }
		__property void set_InheritSettings(bool value) { inheritSettings = value; }
		__property bool get_IsChainedProcess() { return isChainedProcess; }
		__property void set_IsChainedProcess(bool value) { isChainedProcess = value; }
		__property bool get_IsService() { return isService; }
		__property void set_IsService(bool value) { isService = value; }
		__property String *get_ServiceName() { return serviceName; }
		__property void set_ServiceName(String *value) { serviceName = value; }
		__property bool get_IsPaused() 
		{ 
			if (heat && heat->isReady() && !heat->isRunning())
				return true;
			else
				return false;
		}
		__property ArrayList *get_FilteredFunctionList() { return filteredFunctionList; }
		__property void set_FilteredFunctionList(ArrayList *value);
		__property bool get_PauseOnStart() { return pauseOnStart; }
		__property void set_PauseOnStart(bool value) { pauseOnStart = value; }
		__property bool get_ApplicationPaused() 
		{ 
			if (heat) 
				return !heat->isRunning(); 
			else
				return false;
		}
		__property String *get_ApplicationPath() { return applicationToLaunch; }
		__property String *get_ApplicationName();
		__property void set_ApplicationName(String *value) { applicationToLaunch = value; }
		__property String *get_CommandLineParams() { return commandLineParams; }
		__property bool get_ApplicationAttachedTo() { return appAttachedTo; }
		__property bool get_IsRunning();
		__property bool get_SavedLog() { return savedLog; }
		__property void set_SavedLog(bool value) { savedLog = value; }
		__property bool get_TerminateOnClose() { return appNeedsToBeTerminated; }
		__property void set_TerminateOnClose(bool value) { appNeedsToBeTerminated = value; }
		__property String* get_ProjectPath() { return projectPath; }
		__property void set_ProjectPath(String* path) { projectPath = path; }

		__property String* get_ProjectFolder();
		__property String* get_ProjectTitle();

		__property bool get_AttachDebugger() { return attachDebugger; }
		__property void set_AttachDebugger(bool value);
		__property bool get_LogFirstChance() { return logFirstChance; }
		__property void set_LogFirstChance(bool value);
		
		bool OpenLogFile(String *newLogFileName);

		LogItem *GetLogEntryAt(int index, unsigned long &filePosition);
		LogItem *GetLogEntryAtFilePosition(unsigned long filePos);
		void ExportToDiffFile(String *diffFileName, ArrayList *filePositionsToLog);

		void AddExternalDotNetLibrary(String * externalLibrary);

		static bool IsSystemProcess(unsigned long processIDToCheck);
		static bool IsSystemProcess(String *processToCheck);
		static bool IsSystemService(String *serviceNameToCheck, bool canAttachTo);

		bool LaunchApplication(String *ApplicationToLaunch, String *CommandLineParams, bool PauseOnStart, ArrayList *filteredFunctionList, String * exporterLog, bool UseExternalInterceptionList);
		bool LaunchApplication(String *ApplicationToLaunch, String *CommandLineParams, bool PauseOnStart, ArrayList *filteredFunctionList);
		bool AttachApplication(unsigned long processID, bool pauseOnAttach, ArrayList *filteredFunctionList, String * exporterLog, bool UseExternalInterceptionList, bool suspendedApp, bool injectOnlyServiceExeLibrary);
		bool AttachApplication(unsigned long processID, bool pauseOnAttach, ArrayList *filteredFunctionList, bool suspendedApp);
		bool LaunchService(String *serviceToLaunch, bool pauseOnStart, ArrayList *filteredFunctionList);
		bool AttachService(String *serviceToAttachTo, bool pauseOnAttach, ArrayList *filteredFunctionList, bool terminateOnExit);

		void SetAppPauseState(bool value);
		bool PauseApplication();
		bool ResumeApplication();
		bool TerminateApplication();

		bool GetApplicationProcessID(UInt32 &processID);
		bool GetApplicationThreadID(UInt32 &threadID);
		bool GetMonitorInformation(UInt64 &maximumDiskUsage, UInt64 &currentDiskUsage, UInt64 &maximumMemoryUsage, UInt64 &currentMemoryUsage);
		bool GetRealSpaceInformation(UInt64 &maximumDiskUsage, UInt64 &maximumMemoryUsage);
		bool GetNetworkMonitorInformation (UInt64 &uploadUsage, UInt64 &downloadUsage);
		bool GetNetworkUploadBandwidth(UInt64 &uploadBandwidth);
		bool GetNetworkDownloadBandwidth(UInt64 &downloadBandwidth);

		bool SetMaximumDiskUsage(UInt64 maximumDiskUsage, bool isEnabled);
		bool SetMaximumMemoryUsage(UInt64 maximumMemoryUsage, bool isEnabled);
		bool SetNetworkUploadBandwidth(UInt64 maxUploadSpeed,bool IsUploadControlEnabled);
		bool SetNetworkDownloadBandwidth(UInt64 maxDownloadSpeed,bool IsDownloadControlEnabled);
		
		ArrayList *InjectResourceFault(Array *targetThreadID, int faultID, String *resourceName);
		bool RemoveResourceFault(Array *targetThreadID, ArrayList *testIDs);

		bool InjectFault(Array *targetThreadID, int faultID);
		bool RemoveFault(Array *targetThreadID, int faultID);

		void BeginFunctionLoggingUpdate();
		bool EnableFunctionLogging(String *category, String *functionName);
		bool DisableFunctionLogging(String *category, String *functionName);
		void EndFunctionLoggingUpdate();

		bool SetUseProcessChaining(bool useChaining);
		int	GetApplicationThreadList(DWORD * threadID);

		int GetScheduledTestExecutionCount(String * targetThreadID, String * category, String * functionName, String * testID);
		bool CreateScheduledTest(Array *targetThreadID, String *category, FaultFunction *functionTest);
		bool DeleteScheduledTest(Array *targetThreadID, String *testID);
		int SearchInLogFile(String *searchString, int startRecord);
		bool InterceptFunction(String * functionName, String * redirectedFunctionName, String * redirectedDllName);

		bool CreateNetworkCorruptionFault(bool send, bool recv, Array* portList, String* searchRegExpr, String* replacementStr, String **testId);
		bool DeleteNetworkCorruptionFault(String *testId);

		bool CreateFileCorruptionFault(String* fileName, bool alwaysRegen, String* searchRegExpr, String* replacementStr, String **testId);
		bool DeleteFileCorruptionFault(String *testId);
		bool RegenerateFileCorruptionFault(String *fileName);
		bool SetLastFileCorruption(String *fileName, String *corruptedFile, String *changesXmlFile);

		void RemoveOnLogGeneratedHandler(LogsGeneratedDelegate *value);
		void AddOnLogGeneratedHandler(LogsGeneratedDelegate *value);

		void LockOnLogGenerated();
		void UnlockOnLogGenerated();

		String *GetLogFileName() { return logReader->LogFileName; }
		bool DeleteSavedLogFile(String *logFileName);

		void ExceptionThrownHandler(unsigned int processId, ExceptionInfo info);
		String* GetUUID();
	};
}; // namespace