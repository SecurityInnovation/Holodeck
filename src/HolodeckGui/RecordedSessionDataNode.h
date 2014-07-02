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
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace HoloScript;

namespace UIData
{
	public __value enum enableTypes
	{
		disabled,
		modified,
		enabled,
		restartAUT
	};

	public __gc class RecordedSessionDataNode : public DisplayableDataNode
	{
	public:
		__property DisplayableDataNode *get_OriginalNode() { return originalNode; }
		__property DisplayableDataNode *get_ModifiedNode() { return modifiedNode; }
		__property void set_ModifiedNode(DisplayableDataNode *value) { modifiedNode = value; }
		__property LogItem *get_LogItem() { return logItem; }
		__property unsigned int get_LogIndex() { return logIndex; }
		__property UInt64 get_TimeDelta() { return timeDelta; }
		__property void set_TimeDelta(UInt64 value) { timeDelta = value; }
		__property UInt64 get_TimeStamp() { return timeStamp; }
		__property enableTypes get_NodeEnabled() { return nodeEnabled; }
		__property void set_NodeEnabled(enableTypes value) { nodeEnabled = value; }
		__property bool get_NodeReplayed() { return nodeReplayed; }
		__property void set_NodeReplayed(bool value) { nodeReplayed = value; }
		__property String *get_AppName() { return appName; }
		__property void set_AppName(String *value) { appName = value; }
		__property String *get_PropertyName() { return propertyName; }
		__property void set_PropertyName(String *value) { propertyName = value; }
		__property unsigned int get_SessionCount() { return sessionCount; }
		__property void set_SessionCount(unsigned int value) { sessionCount = value; }


		RecordedSessionDataNode(DisplayableDataNode *dataNode, LogItem *log, unsigned int LogIndex);
		~RecordedSessionDataNode();

	private:
		enableTypes nodeEnabled;
		DisplayableDataNode *originalNode;  //points to the test, limit, or fault this node represents
		DisplayableDataNode *modifiedNode;	//optionally points to a modified data node
		HoloScript::LogItem *logItem;
		unsigned int logIndex;
		UInt64 timeDelta;
		UInt64 timeStamp;
		bool nodeReplayed;
		String *appName;
		String *propertyName;				//if the node was modified this holds the property modified, otherwise null
		unsigned int sessionCount;			//how many restarts have there been?
	};
}