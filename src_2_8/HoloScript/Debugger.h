//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Debugger.h
//
// DESCRIPTION: Contains definition for the Holodeck debugger
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 FEB 2004		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include <dbghelp.h>
#using <mscorlib.dll>

using namespace System;
using namespace System::Threading;

namespace HoloScript
{
	public __value struct ExceptionInfo
	{
		String* timeStamp;
		bool firstChance;
		unsigned int threadId;
		unsigned int exceptionCode;
		unsigned int exceptionAddress;
		bool writeAccess;
		unsigned int memoryAddress;

		String* miniDumpFile;
	};

	public __delegate void ExceptionThrownDelegate(unsigned int processID, ExceptionInfo info);

	public __gc class HoloScriptApplication;

	public __gc class HoloScriptDebugger
	{
	private:
		void DebugThreadProc();
		bool GenerateMinidump(DWORD threadId, EXCEPTION_RECORD* exceptionRecord, wchar_t* fileName);

		HoloScriptApplication* attachedApp;
		Thread* debugThread;
		unsigned int processId;
		bool terminateDebugThread;
		bool logFirstChance;
		HMODULE hDbgHelpDll;
		BOOL (__stdcall *MiniDumpWriteDumpFunc)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
			PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);

	public:
		HoloScriptDebugger();
		~HoloScriptDebugger();

		bool Attach(HoloScriptApplication* app);
		bool Detach();

		ExceptionThrownDelegate* OnExceptionThrown;

		__property bool get_LogFirstChance() { return logFirstChance; }
		__property void set_LogFirstChance(bool value) { logFirstChance = value; }
	};
}
