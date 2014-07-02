//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReader.cpp
//
// DESCRIPTION: Contains implementation for the class LogReader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogReader.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		LogReader
	// Description: Constructor for the LogReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReader::LogReader() : totalLogCount(0)
	{
		LogsCache = new ArrayList();
		grabbedLogEntries = new Queue();
		logInfoList = new ArrayList();
		logIndexInfoTable = new Hashtable();
		logFileName = new String("");

		// Initialize GetLogEntryFromFile cache
		cachedLogOffsets = NULL;
		cachedLogInfoPtr = NULL;
	}

	//*************************************************************************
	// Method:		~LogReader
	// Description: Destructor for the LogReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReader::~LogReader()
	{
		CloseHandle(logFile);
	}	

}