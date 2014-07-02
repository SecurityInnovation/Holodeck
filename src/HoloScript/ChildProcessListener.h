//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ChildProcessListener.h
//
// DESCRIPTION: Contains the definition for the class ChildProcessListener
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include <windows.h>
#include "defines.h"
#include "Event.h"
#include "MemoryMappedFile.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::Threading;
using namespace System::Collections;
using namespace Microsoft::Win32;
using namespace SiUtils;

namespace HoloScript
{
	public __delegate void ChildProcessCreatedDelegate(bool newProcess, unsigned long id, bool resume);

	//*************************************************************************
	// class to provide a mechanism to listen for new child process logs
	//*************************************************************************
	public __gc class ChildProcessListener
	{
	protected:
		Event *childProcessCreatedEvent;
		Event *holodeckAttachedToChildProcessEvent;
		MemoryMappedFile *childProcessDataFile;

		Thread *childProcessCreatedListenerThread;
		bool listenThreadRunning;
		void ListenThreadFunction();

	public:
		ChildProcessListener(unsigned long procID);
		~ChildProcessListener();

		ChildProcessCreatedDelegate *OnChildProcessCreated;

		void StartListeningForChildProcesses();
	};
}