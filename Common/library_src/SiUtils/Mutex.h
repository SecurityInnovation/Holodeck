//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Mutex.h
//
// DESCRIPTION: Contains definition for the utility class Mutex
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
	const int MUTEX_SIGNALED = WAIT_OBJECT_0;
	const int MUTEX_TIMED_OUT = WAIT_TIMEOUT;
	const int MUTEX_ABANDONED = WAIT_ABANDONED;

	//*************************************************************************
	// Encapsulates a Windows kernel mutex object
	//*************************************************************************
	class Mutex
	{
	private:
		HANDLE handle;
	public:
		Mutex();
		~Mutex();

		bool Create(char *name = NULL);
		bool Create(const SiString &name);

		bool Open(char *name = NULL);
		bool Open(const SiString &name);

		bool Close();

		unsigned int Acquire(long waitCount = INFINITE);
		void Release();
	};
}
