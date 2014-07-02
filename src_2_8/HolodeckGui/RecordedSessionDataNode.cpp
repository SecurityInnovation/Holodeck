//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RecordedSessionDataNode.h
//
// DESCRIPTION: Contains definition for the class RecordedSessionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "RecordedSessionDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		RecordedSessionDataNode
	// Description: Constructor for the RecordedSessionDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	RecordedSessionDataNode::RecordedSessionDataNode(DisplayableDataNode *dataNode, HoloScript::LogItem *log, unsigned int LogIndex)
	{
		SYSTEMTIME currentTime;
		FILETIME fileCurrentTime;
		ULARGE_INTEGER ulargeCurrentTime;
		UInt64 uInt64LogTime;
		UInt64 uInt64CurrentTime;

		InternalConstructor(dataNode);

		ID = "";
		originalNode = dataNode;
		logItem = log;
		logIndex = LogIndex;
		nodeEnabled = enabled;
		nodeReplayed = false;
	
		GetLocalTime(&currentTime);
		SystemTimeToFileTime(&currentTime, &fileCurrentTime);
		ulargeCurrentTime.LowPart = fileCurrentTime.dwLowDateTime;
		ulargeCurrentTime.HighPart = fileCurrentTime.dwHighDateTime;
		uInt64CurrentTime = ulargeCurrentTime.QuadPart;

		timeStamp = uInt64CurrentTime;

		if (log)
		{
			uInt64LogTime = log->UInt64TimeStamp;
			timeDelta = uInt64CurrentTime - uInt64LogTime;
		}
		else
			timeDelta = 0;

	}

	//*************************************************************************
	// Method:		~RecordedSessionDataNode
	// Description: Destructor for the RecordedSessionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RecordedSessionDataNode::~RecordedSessionDataNode()
	{
	}
}