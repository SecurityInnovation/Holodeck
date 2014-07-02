//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogQueue.h
//
// DESCRIPTION: Contains the definition for the class LogQueue
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "defines.h"

#ifdef LogDll_File 
#define HolodeckLog_Api __declspec(dllexport)
#else
#define HolodeckLog_Api __declspec(dllimport)
#endif

namespace Log
{
	//*************************************************************************
	// class to hold a collection of log data
	//*************************************************************************
	class HolodeckLog_Api LogQueue
	{
	private:
		static HANDLE heap;

		unsigned char *byteSequences[MAX_LOG_QUEUE_SIZE];
		unsigned int byteSequenceSizes[MAX_LOG_QUEUE_SIZE];
		int numQueueEntries;
		int headPosition;
		int tailPosition;

		CRITICAL_SECTION queueGuard;

	public:
		LogQueue();
		~LogQueue();

		int GetLogCount();
		bool IsFull();
		bool IsEmpty();

		int GetTotalLogSizes();

		int GetNextLogSize();
		bool PushLog(unsigned int size, unsigned char *logData);
		bool PopLog(unsigned int size, unsigned char *logData);
	};
}
