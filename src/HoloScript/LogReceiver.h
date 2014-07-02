//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReceiver.h
//
// DESCRIPTION: Contains the definition for the class LogReceiver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include <windows.h>
#include "defines.h"
#include "Event.h"
#include "MemoryMappedFile.h"
#include "NotifiableLogReceiver.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::Threading;
using namespace System::Collections;
using namespace Microsoft::Win32;
using namespace SiUtils;

namespace HoloScript
{
	//*************************************************************************
	// class to provide a mechanism to receive logs from a LogSender object
	//*************************************************************************
	public __gc class LogReceiver
	{
	private:
		ArrayList *listeners;
		
		Event *logsSentEvent;
		Event *logsReceivedEvent;

		MemoryMappedFile *entryCountFile;

		Thread *logReceivedThread;
		bool receiveLogThreadRunning;
		void ReceiveLogThreadFunction();

		void NotifyLogReceivedListeners(ReceivedLogInfo *info);
	public:
		LogReceiver(int procID);
		~LogReceiver();

		void StartReceivingLogs();

		void AddLogReceivedListener(NotifiableLogReceiver *receiver);
		void RemoveLogReceivedListener(NotifiableLogReceiver *receiver);
	};
}