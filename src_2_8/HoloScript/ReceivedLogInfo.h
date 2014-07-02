//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReceivedLogInfo.h
//
// DESCRIPTION: Contains definition for the ReceivedLogInfo class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>

#include "LogItem.h"
#include "ManagedEnums.h"

namespace HoloScript
{
	//*************************************************************************
	// entity class to hold information received from a log sender object
	//*************************************************************************
	public __gc class ReceivedLogInfo
	{
	private:
		unsigned long logFilePos;
		int numLogEntries;
		int startIndex;
		Holodeck::HolodeckVersion version;

	public:
		ReceivedLogInfo();
		~ReceivedLogInfo();

		__property unsigned long get_FilePosition() { return logFilePos; }
		__property int get_LogEntryCount() { return numLogEntries; }
		__property int get_StartIndex() { return startIndex; }
		__property Holodeck::HolodeckVersion get_Version() { return version; }

		__property void set_FilePosition(unsigned long value) { logFilePos = value; }
		__property void set_LogEntryCount(int value) { numLogEntries = value; }
		__property void set_StartIndex(int value) { startIndex = value; }
		__property void set_Version(Holodeck::HolodeckVersion value) { version = value; }
	};
}