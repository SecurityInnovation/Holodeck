//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExporterGui.cpp
//
// DESCRIPTION: File containing the definitions for TestApplication class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 02 May 2003		 A. Kakrania	File created.
//*************************************************************************
#pragma once
#using <mscorlib.dll>
#include <windows.h>

#using "HoloScript.dll"
#using "FunctionDatabase.dll"

using namespace System;
using namespace System::Collections;

using namespace HoloScript;
using namespace FunctionDatabase;


namespace HolodeckLib
{
	public __gc class TestApplication
	{
	public:
		TestApplication();
		~TestApplication();

		bool LaunchApplication(String * applicationName, String * cmdLine, bool startPaused);
		bool AttachApplication(unsigned long processID, bool pauseOnAttach);
		bool LaunchService(String *serviceName, bool startPaused);
		bool AttachService(String *serviceName, bool startPaused);
		bool InterceptFunction (String * functionName, String * replacementFunctionName, String * replacementDllName);

		bool PauseApplication();
		bool ResumeApplication();
		bool TerminateApplication();

		bool GetApplicationProcessID(UInt32 &processID);
		bool GetApplicationThreadID(UInt32 &threadID);

		bool SetMaximumDiskUsage(UInt64 maximumDiskUsage, bool isEnabled);
		bool SetMaximumMemoryUsage(UInt64 maximumMemoryUsage, bool isEnabled);
		bool SetNetworkUploadSpeed(UInt64 uploadSpeed, bool isEnabled);
		bool SetNetworkDownloadSpeed(UInt64 downloadSpeed, bool isEnabled);
		
		bool InjectFault(Array *targetThreadID, int faultID);
		bool RemoveFault(Array *targetThreadID, int faultID);

		bool EnableFunctionLogging(String *category, String *functionName);
		bool DisableFunctionLogging(String *category, String *functionName);

		bool SetUseProcessChaining(bool useChaining);

		void SetDotNetLibraryInitList(ArrayList * externalLibsList);


	private:
		ArrayList * interceptionList;
		HoloScriptApplication * holoApp;
	};
}
