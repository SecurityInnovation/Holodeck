//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class LogPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <HoloScript.dll>
#using <SIControls.dll>

#include "DisplayableDataArray.h"
#include "DisplayableDataNode.h"
#include "LogPaneDataNode.h"
#include "LogEntryIndexer.h"
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
	public __delegate bool AutoFilterDelegate(LogItem * item);
	public __delegate void LogEntrySetReceivedDelegate(unsigned int numEntriesInSet);
	public __delegate void LoggedNewThreadIDDelegate(unsigned int processID, unsigned int threadID);

	public __gc class LogPaneDataArray : public DisplayableDataArray
	{
	public:
		LogPaneDataArray(String *executableFileName, String *logFilePath, int oldProcId, String *exeAndLogSaveTime);
		LogPaneDataArray(String *executableFileName, unsigned int processID, unsigned int threadID);
		~LogPaneDataArray();
		void CleanUp();

		void OpenSavedLog();

		AutoFilterDelegate *FilterLogItem;
		LogEntrySetReceivedDelegate *LogEntrySetReceived;
		LoggedNewThreadIDDelegate *LoggedNewThreadID;

		[Description("The Total number of log entries for this application"), Category("Owner Info")]
		__property int get_TotalApplicationLogEntries() { return (holoScriptApp ? holoScriptApp->LogEntryCount : 0); }
		[Browsable(false)]
		__property int get_TotalLogEntries() { return totalEntries; }
		[Browsable(false)]
		__property int get_OtherLogEntries() { return this->otherEntries; }
		[Description("True if the application is paused, false if it is not"), Category("Owner Info")]
		__property bool get_IsPaused() { return holoScriptApp->IsPaused; }
		[Description("True if the application is still running, false if it is not"), Category("Owner Info")]
		__property bool get_IsRunning() { return holoScriptApp->IsRunning; }
		[Description("The filename of the executable for this application"), Category("Owner Info")]
		__property String *get_ExecutableFileName() { return executableFileName; }
		[Description("The full path to the executable for this application"), Category("Owner Info")]
		__property String *get_ExecutablePath();
		[Browsable(false)]
		__property String *get_FullExecutableName();
		[Description("The size of the executable file for this application"), Category("Owner Info")]
		__property __int64 get_ExecutableSize();
		[Description("The time that this application started"), Category("Owner Info")]
		__property DateTime get_StartTime();
		[Description("The elapsed time that this application has been executing"), Category("Owner Info")]
		__property TimeSpan get_ElapsedTime();

		[Browsable(false)]
		__property unsigned int get_VisibleAndExpandedCount() { return (indexer ? indexer->VisibleAndExpandedCount : 0); }

		[Browsable(false)]
		__property String * get_SavedLogFilePath() { return savedLogFilePath; }
		[Browsable(false)]
		__property String * get_ExeAndLogSavedTime() { return exeAndLogSavedTime; }

		[Browsable(false)]
		__property HoloScriptApplication *get_HoloScriptApplicationInstance() { return holoScriptApp; }

		[Browsable(false)]
		__property unsigned int get_SortColumn() { return indexer->SortColumn; }
		__property void set_SortColumn(unsigned int value) { indexer->SortColumn = value; }

		[Browsable(false)]
		__property unsigned int get_SortOrder() { return indexer->SortOrder; }
		__property void set_SortOrder(unsigned int value) { indexer->SortOrder = value; }

		[Browsable (false)]
		__property int get_Count() { return indexer->Count; }

		[Browsable (false)]
		__property int get_CachedLogIndex() { return cachedLogItemIndex; }

		[ReadOnly(true), Description("True if the log is a saved log, false if it is an active log"), Category("Owner Info")]
		__property bool get_SavedLog() { return holoScriptApp->SavedLog; }
		__property void set_SavedLog(bool value) { holoScriptApp->SavedLog = value; }

		[Browsable(false)]
		__property TreeListNode *get_RootNode();

		[Browsable(false)]
		__property unsigned long get_VisibleNodeCount() { return rootNode ? GetChildNodeCount(rootNode) : 0; }

		void SortItems();
		LogItem *GetLogItemAt(int listViewIndex);
		LogItem *GetLogItemForIndexerNode(IndexerNode *node) { return (indexer ? indexer->GetLogEntryFromNode(node) : NULL); }
		int GetIndexFromFilePosition(unsigned long filePos) { return indexer->GetIndexFromFilePosition(filePos); }
		LogPaneDataNode *GetSortedDataNodeAt(int index);
		bool GetParentIndex(unsigned int index, unsigned int *parentIndex);
		unsigned int FilterCurrentItems();
		void ExportSavedLogItemsToFile(String *fileName);
		TreeListNode *GetTreeListNodeAtIndex(unsigned int index);
		TreeListNode *GetTreeListNodeAtRow(unsigned int row);
		LogItem *GetLogItemAtIndex(unsigned int index);
		void UnselectAllNodes();
		bool DeleteSavedLog();
		void ExportDataToFile(String *fileName);

		void LogPaneAttach();

	protected:
		bool isLoaded;
		unsigned long totalEntries;
		unsigned long currentEntries;
		unsigned long otherEntries;
		String *executableFileName;
		HoloScriptApplication *holoScriptApp;
		System::Diagnostics::Process *process;
		LogEntryIndexer *indexer;
		int cachedLogItemIndex;
		LogItem* cachedLogItem;
		static System::Threading::Mutex* listMutex = new System::Threading::Mutex(false);
		String *savedLogFilePath;
		String *exeAndLogSavedTime;
		HoloScript::LogsGeneratedDelegate *logHandler;
		IndexerNode *internalLevelZeroNode;
		IndexerNode *rootNode;
		TreeListNode *rootTreeListNode;
		bool logPaneAttached;

		Hashtable *threadIDHashTable;

		GetParentNodeDelegate *GetParentNodeHandler;
		GetFirstChildDelegate *GetFirstChildHandler;
		GetLastChildDelegate *GetLastChildHandler;
		GetNextChildDelegate *GetNextChildHandler;
		GetPrevChildDelegate *GetPrevChildHandler;
		GetNextSiblingDelegate *GetNextSiblingHandler;
		GetPrevSiblingDelegate *GetPrevSiblingHandler;
		GetChildNodeAtDelegate *GetChildNodeAtHandler;

		SetIsSelectedDelegate *SetIsSelectedHandler;
		SetIsFocusedDelegate *SetIsFocusedHandler;
		SetIsExpandedDelegate *SetIsExpandedHandler;

		GetRowNumberDelegate *GetRowNumberHandler;
		GetNodeIndexDelegate *GetNodeIndexHandler;

		GetChildNodeCountDelegate *GetChildNodeCountHandler;
		GetVisibleNodeCountDelegate *GetVisibleNodeCountHandler;

		void Initialize(String *ExecutableFileName, String *logFilePath, unsigned int processID, unsigned int threadID, String *ExeAndLogSaveTime, HoloScriptApplication *hsApp);
		void InitTreeListNodeDelegates(TreeListNode *node);
		
		ParentChildList *onGetParentNode(TreeListNode *node);
		ParentChildList *onGetFirstChildNode(TreeListNode *node);
		ParentChildList *onGetLastChildNode(TreeListNode *node);
		ParentChildList *onGetNextChildNode(TreeListNode *node, TreeListNode *curChild);
		ParentChildList *onGetPrevChildNode(TreeListNode *node, TreeListNode *curChild);
		TreeListNode *onGetChildNodeAt(TreeListNode *node, unsigned int index);

		void onSetIsSelected(TreeListNode *node, bool selected);
		void onSetIsExpanded(TreeListNode *node, bool expanded);
		void onSetIsFocused(TreeListNode *node, bool focused);

		ParentChildList *onGetNextSiblingNode(TreeListNode *node);
		ParentChildList *onGetPrevSiblingNode(TreeListNode *node);
		
		int onGetChildNodeCount(TreeListNode *node);
		int onGetRowNumber(TreeListNode *node, bool visibleOnly);
		int onGetNodeIndex(TreeListNode *parent, TreeListNode *child);
		int GetVisibleNodeCount(IndexerNode *node);
		int GetChildNodeCount(IndexerNode *node);
		int onGetVisibleNodeCount(TreeListNode *node);

		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
		bool AddLogItemToBufferedTreeStructure(LogItem *item, bool itemFiltered);
		void AddBufferedTreeStructureToIndexer(IndexerNode *node, unsigned int *numAdded, unsigned int *filtered);
		void AddNodeToIndexer(IndexerNode *node, unsigned int *numAdded, unsigned int *filtered);

		TreeListNode *GetTreeListNodeFromIndexerNode(IndexerNode* node);

		IndexerNode* TagToIndexerNode(Object* tag);
	};
}