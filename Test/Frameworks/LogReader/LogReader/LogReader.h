//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReader.h
//
// DESCRIPTION: Contains definition for the class LogReader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "NotifiableLogReceiver.h"
#include "LogItem.h"
#include "ReceivedLogInfo.h"
//#include "Mutex.h"
#include "defines.h"
//#include "ManagedEnums.h"
#include <vcclr.h>

#using <mscorlib.dll>
#using <System.dll>

using namespace System;
using namespace System::IO;
//using namespace SiUtils;
//using namespace Holodeck;

namespace HoloScript
{
	public __delegate void LogsGeneratedDelegate(int numNewLogs, LogItem* newLogItems __gc[], unsigned int pID, unsigned int tID);

	public __gc class LogReader //: public NotifiableLogReceiver
	{
	public:
		LogReader();
		~LogReader();

		LogsGeneratedDelegate *OnLogGenerated;

		__property virtual unsigned long get_LogEntryCount() { return this->totalLogCount; }

		__property virtual String *get_LogFileName() { return logFileName; }
		__property virtual void set_LogFileName(String *value) { logFileName = value; }

		__property void set_ProcessID(unsigned int value) { processID = value; }

			
	protected:		
		HANDLE logFile;						// the log file to get logs from

		unsigned int *cachedLogOffsets;		// Cached log offsets for GetLogEntryFromFile
		ReceivedLogInfo *cachedLogInfoPtr;	// ReceivedLogInfo that is cached for GetLogEntryFromFile
		ArrayList *LogsCache;				// Temp storage for logs received before log pane is fully intialized

		unsigned long totalLogCount;		// total number of logs in the file
		String *logFileName;				// the path to the log file

		Queue *grabbedLogEntries;			// a queue of LogItem* objects
		ArrayList *logInfoList;				// an array of ReceivedLogInfo* objects
		Hashtable *logIndexInfoTable;		// a hashtable mapping log entry indices to received log info pointers

		unsigned int processID;
		
	};
}