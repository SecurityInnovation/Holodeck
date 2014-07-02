//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HolodeckLib.cpp
//
// DESCRIPTION: File containing the implementation for TestApplication class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 02 May 2003		 A. Kakrania	File created.
//*************************************************************************
#include "HolodeckLib.h"

using namespace HoloScript;
using namespace FunctionDatabase;

namespace HolodeckLib
{
	//*************************************************************************
	// Method:		TestApplication
	// Description: Constructor for the TestApplication class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestApplication::TestApplication()
	{
		interceptionList = new ArrayList();
		holoApp = new HoloScriptApplication ();
	}

	//*************************************************************************
	// Method:		~TestApplication
	// Description: Destructor for the TestApplication class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestApplication::~TestApplication()
	{
	}

	//*************************************************************************
	// Method:		InterceptFunction
	// Description: Adds the function details to a list which is used when Launch or
	//				Attach operations are performed.
	//
	// Parameters:
	//	functionName	 - the name of the function
	//	replacementFunctionName - the name of the replacement function
	//	replacementDllName - the name of the replacement library
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::InterceptFunction (String * functionName, String * replacementFunctionName, String * replacementDllName)
	{
		return holoApp->InterceptFunction (functionName, replacementFunctionName, replacementDllName);
	}

	//*************************************************************************
	// Method:		LaunchApplication
	// Description: Launches an application
	//
	// Parameters:
	//	applicationName	 - the name and path of the application to intercept
	//	cmdLine - command line arguments to pass to the aut
	//	logFile - the file where the logs should be stored
	//	startPaused - true if app should be paused on startup, false otherwise
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::LaunchApplication(String * applicationName, String * cmdLine, bool startPaused)
	{
		return holoApp->LaunchApplication (applicationName, cmdLine, startPaused, NULL, NULL, true);
	}

	//*************************************************************************
	// Method:		AttachApplication
	// Description: Attaches the heat engine to an already running application
	//
	// Parameters:
	//	processID - the process ID of the application to attach to
	//	logFile - the file where the logs should be stored
	//	pauseOnAttach - true if app should be paused after injection, false otherwise
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::AttachApplication(unsigned long processID, bool pauseOnAttach)
	{
		return holoApp->AttachApplication (processID, pauseOnAttach, NULL, NULL, true, false, false);
	}

	//*************************************************************************
	// Method:		LaunchService
	// Description: Starts a service and attaches heat to it
	//
	// Parameters:
	//	serviceName - the name of the service to attach to
	//	startPaused - true to pause the service on attach, false otherwise
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::LaunchService(String *serviceName, bool startPaused)
	{
		return holoApp->LaunchService(serviceName, startPaused, NULL);
	}

	//*************************************************************************
	// Method:		AttachService
	// Description: Attaches the heat engine to an already running service
	//
	// Parameters:
	//	serviceName - the name of the service to attach to
	//	startPaused - true to pause the service on attach, false otherwise
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::AttachService(String *serviceName, bool startPaused)
	{
		return holoApp->AttachService(serviceName, startPaused, new ArrayList(), false);
	}

	//*************************************************************************
	// Method:		PauseApplication
	// Description: Pauses the test application
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::PauseApplication()
	{
		return holoApp->PauseApplication();
	}

	//*************************************************************************
	// Method:		ResumeApplication
	// Description: Resumes an already paused test application
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::ResumeApplication()
	{
		return holoApp->ResumeApplication();
	}

	//*************************************************************************
	// Method:		TerminateApplication
	// Description: Terminates the test application
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::TerminateApplication()
	{
		return holoApp->TerminateApplication();
	}

	//*************************************************************************
	// Method:		GetApplicationProcessID
	// Description: Gets the processID of the test application
	//
	// Parameters:
	//	processID - [out] receives the processID
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::GetApplicationProcessID(UInt32 &processID)
	{
		return holoApp->GetApplicationProcessID(processID);
	}

	//*************************************************************************
	// Method:		GetApplicationThreadID
	// Description: Gets the main threadID of the test application
	//
	// Parameters:
	//	threadID - [out] receives the threadID
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::GetApplicationThreadID(UInt32 &threadID)
	{
		return holoApp->GetApplicationThreadID(threadID);
	}

	//*************************************************************************
	// Method:		SetMaximumDiskUsage
	// Description: Sets the maximum amount of diskspace available to the aut
	//
	// Parameters:
	//	maximumDiskUsage - the limit
	//	isEnabled - true to enable the limit, false to disable
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::SetMaximumDiskUsage(UInt64 maximumDiskUsage, bool isEnabled)
	{
		return holoApp->SetMaximumDiskUsage (maximumDiskUsage, isEnabled);
	}

	//*************************************************************************
	// Method:		SetMaximumMemoryUsage
	// Description: Sets the maximum amount of memory available to the aut
	//
	// Parameters:
	//	maximumMemoryUsage - the limit
	//	isEnabled - true to enable the limit, false to disable
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::SetMaximumMemoryUsage(UInt64 maximumMemoryUsage, bool isEnabled)
	{
		return holoApp->SetMaximumMemoryUsage (maximumMemoryUsage, isEnabled);
	}
	
	//*************************************************************************
	// Method:		SetNetworkUploadSpeed
	// Description: Sets the maximum speed for network upload
	//
	// Parameters:
	//	uploadSpeed - upload limit in bytes/sec
	//	isEnabled - true to enable the limit, false to disable
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::SetNetworkUploadSpeed(UInt64 uploadSpeed, bool isEnabled)
	{
		return holoApp->SetNetworkUploadBandwidth (uploadSpeed, isEnabled);
	}

	//*************************************************************************
	// Method:		SetNetworkDownloadSpeed
	// Description: Sets the maximum speed for network download
	//
	// Parameters:
	//	downloadSpeed - download limit in bytes/sec
	//	isEnabled - true to enable the limit, false to disable
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::SetNetworkDownloadSpeed(UInt64 downloadSpeed, bool isEnabled)
	{
		return holoApp->SetNetworkDownloadBandwidth (downloadSpeed, isEnabled);
	}

	//*************************************************************************
	// Method:		InjectFault
	// Description: Injects a fault into the AUT
	//
	// Parameters:
	//	targetThreadID - an array of threads to inject the fault into; if NULL
	//					 fault is injected into all threads
	//	faultID - the type of fault to inject
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::InjectFault(Array *targetThreadID, int faultID)
	{
		return holoApp->InjectFault (targetThreadID, faultID);
	}

	//*************************************************************************
	// Method:		RemoveFault
	// Description: Removes an injected fault
	//
	// Parameters:
	//	targetThreadID - an array of threads to remove the fault from; if NULL
	//					 fault is removed from the global thread key
	//	faultID - the type of fault to inject
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::RemoveFault(Array *targetThreadID, int faultID)
	{
		return holoApp->RemoveFault (targetThreadID, faultID);
	}

	//*************************************************************************
	// Method:		EnableFunctionLogging
	// Description: Enables logging of a function
	//
	// Parameters:
	//	category - The catrgory that the function belongs to
	//	functionName - the function name
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::EnableFunctionLogging(String *category, String *functionName)
	{
		return holoApp->EnableFunctionLogging (category, functionName);
	}
	
	//*************************************************************************
	// Method:		DisableFunctionLogging
	// Description: Disables logging of a function
	//
	// Parameters:
	//	category - The catrgory that the function belongs to
	//	functionName - the function name
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::DisableFunctionLogging(String *category, String *functionName)
	{
		return holoApp->DisableFunctionLogging (category, functionName);
	}

	//*************************************************************************
	// Method:		SetUseProcessChaining
	// Description: Control whether process chaining should take place or not
	//
	// Parameters:
	//	useChaining - true means process chaining should be used; false means otherwise
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool TestApplication::SetUseProcessChaining(bool useChaining)
	{
		return holoApp->SetUseProcessChaining (useChaining);
	}

	//*************************************************************************
	// Method:		SetDotNetLibraryInitList
	// Description: Sets the list of external libraries which should be initialized
	//				for DotNet interception.
	//
	// Parameters:
	//	externalLibsList - the list of user specified dotnet replacement libraries
	//
	// Return Value: (None)
	//*************************************************************************
	void TestApplication::SetDotNetLibraryInitList(ArrayList * externalLibsList)
	{
		if (!externalLibsList || externalLibsList->Count < 1)
			return;

		String * tmpString;
		for (int i=0; i < externalLibsList->Count; i++)
		{
			tmpString = dynamic_cast <String *> (externalLibsList->Item[i]);
			holoApp->AddExternalDotNetLibrary (tmpString);
		}
	}
}


// Dummy DLLMain to prevent any automatic execution of managed code
BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:;
		case DLL_PROCESS_DETACH:;
		case DLL_THREAD_ATTACH:;
		case DLL_THREAD_DETACH:;
		default:;
	}
    return TRUE;
}