//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class NetworkLogPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <HoloScript.dll>
#using <SIControls.dll>

#include "DisplayableDataArray.h"
#include "DisplayableDataNode.h"
#include "NetworkLogPaneDataNode.h"
#include "LogPaneDataArray.h"
#include "UIEventRegistry.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace HoloScript;
using namespace SIControls;

namespace UIData
{
	public __value enum NetworkLogItemFields
	{
		TimeStampField,
		ThreadField,
		IncomingField,
		ProtocolField,
		MessageLengthField,
		OriginalMessageLengthField,
		DestinationIPField,
		DestinationPortField,
		SourceIPField,
		SourcePortField,
		MessagePreviewField
	};

	public __gc struct NetworkLogIndexNode
	{
		unsigned long filePos; // the file positions of the log entries (not network log entry) so we can grab them from the indexer easily later
		bool selected;
		bool focused;
		UInt64 timeStamp;
		String* cachedSortColumnEntry;
	};

	public __delegate bool AutoNetworkLogFilterDelegate(NetworkLogItem * item);

	public __gc class NetworkLogPaneDataArray : public DisplayableDataArray, public IComparer
	{
	public:
		NetworkLogPaneDataArray(unsigned int processID, unsigned int threadID);
		NetworkLogPaneDataArray(unsigned int processID, unsigned int threadID, HoloScriptApplication *hsApp);
		NetworkLogPaneDataArray(unsigned int processID, unsigned int threadID, String *logFilePath, HoloScriptApplication *hsApp);
		~NetworkLogPaneDataArray();
		void CleanUp();

		AutoNetworkLogFilterDelegate *FilterLogItem;
		LogEntrySetReceivedDelegate *LogEntrySetReceived;

		[Description("The Total number of log entries for this Active Network Log set"), Category("Owner Info")]
		__property int get_TotalLogEntries() { return logIndexNodes->Count + filteredOutList->Count; }

		[Browsable(false)]
		__property unsigned int get_VisibleLogEntries() { return logIndexNodes ? logIndexNodes->Count : 0; }

		[Browsable(false)]
		__property int get_OtherLogEntries() { return this->otherEntries; }

		[Browsable(false)]
		__property unsigned int get_SortColumn() { return sortColumn; }
		__property void set_SortColumn(unsigned int value);

		[Browsable(false)]
		__property unsigned int get_SortOrder() { return sortOrder; }
		__property void set_SortOrder(unsigned int value) { sortOrder = value; }

		[Browsable(false)]
		__property String * get_SavedLogFilePath() { return savedLogFilePath; }

		[ReadOnly(true), Description("True if the log is a saved netowork log, false if it is an active network log"), Category("Owner Info")]
		__property bool get_SavedLog() { return isSavedLog; }
		__property void set_SavedLog(bool value) { isSavedLog = value; }

		void SortItems();
		NetworkLogItem *GetLogItemAt(int listViewIndex);
		unsigned int FilterCurrentItems();
		NetworkLogPaneDataNode *GetSortedDataNodeAt(int index);
		unsigned long GetFilePositionAt(int listViewIndex);
		int GetIndexOfFilePosition(unsigned long filePos);

		bool IsSelected(int listViewIndex);
		void SetIsSelected(int listViewIndex, bool selected);
		bool IsFocused(int listViewIndex);
		void SetIsFocused(int listViewIndex, bool focused);

		int Compare(Object *obj1, Object *obj2);

		void ExportToFile(String * exportFileName);
		bool DeleteSavedLog();

	protected:
		unsigned long totalEntries;
		unsigned long currentEntries;
		unsigned long otherEntries;
		unsigned int sortColumn;
		unsigned int sortOrder;
		int cachedLogItemIndex;
		ArrayList *logIndexNodes;
		ArrayList *filteredOutList;
		String *savedLogFilePath;
		bool isSavedLog;
		HoloScriptApplication *holoScriptApp;
		HoloScript::LogsGeneratedDelegate *logHandler;
		static System::Threading::Mutex* listMutex = new System::Threading::Mutex(false);
		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
		void UpdateSortColumnCache(NetworkLogIndexNode* node, LogItem* logItem, NetworkLogItem *netLogItem);
		void AddSortedNode(NetworkLogIndexNode *node);
		void InternalConstructor(unsigned int processID, unsigned int threadID, HoloScriptApplication *hsApp, String * savedNetworkFileName);
		String * getFormattedAddressString(String *ip, String * port);
	};
}