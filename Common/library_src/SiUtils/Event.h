//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Event.h
//
// DESCRIPTION: Contains definition for the utility class Event
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 JAN 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include <windows.h>
#include "SiString.h"

namespace SiUtils
{
	const int EVENT_SIGNALED = WAIT_OBJECT_0;
	const int EVENT_TIMED_OUT = WAIT_TIMEOUT;

	//*************************************************************************
	// Encapsulates a Windows kernel event object
	//*************************************************************************
	class Event
	{
	private:
		HANDLE handle;
	public:
		Event();
		~Event();

		bool IsValidEvent() { return ((handle != NULL) && (handle != INVALID_HANDLE_VALUE)); }

		bool Create(char *name = NULL, bool initialState = false, bool manualReset = false);
		bool Create(const SiString &name, bool initialState = false, bool manualReset = false);

		bool Open(char *name = NULL);
		bool Open(const SiString &name);

		bool OpenExisting(char *name = NULL);
		bool OpenExisting(const SiString &name);

		bool Close();

		unsigned int Acquire(long waitCount = INFINITE);
		void Release();
		void Signal();

		static unsigned int AcquireMultiple(Event** eventArray, int eventCount, bool waitAll, long waitCount = INFINITE);
	};
}