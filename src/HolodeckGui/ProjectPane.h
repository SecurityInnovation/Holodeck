//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectPane.h
//
// DESCRIPTION: Contains definition for the class ProjectPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "DockablePane.h"
#include "ProjectPaneDataArray.h"
#include "LogPaneDataArray.h"
#include "FaultPaneDataArray.h"
#include "ReportPaneDataArray.h"
#include "RecordedSessionDataArray.h"
#include "ScheduledTestDataArray.h"
#include "Defines.h"
#include "ImageListManager.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace UIData;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Threading;


namespace HolodeckGui
{
	public __gc class ProjectPane : public DockablePane
	{
	public:
		ProjectPane(ProjectPaneDataArray *paneData);
		~ProjectPane();
		void Dispose (bool disposing);

		__property TreeNode* get_SelectedNode() {return projectTreeView->SelectedNode;}

	private:
		__delegate void AddNodeDelegate(DisplayableDataNode *dataNode, int position);
		__delegate void DeleteNodeDelegate(DisplayableDataNode *dataNode);
		AddNodeDelegate *addNodeDelegate;
		DeleteNodeDelegate *deleteNodeDelegate;

		static Mutex *firstUseMutex = new Mutex(false);
		SortedList *processMutexList;
		SortedList *threadMutexList;
		bool firstUse;
		bool ignoreFocusChange;
		bool perThread;
		String *replayMenuItemText;
		String *recordMenuItemText;
		bool replayMenuItemEnabled;
		bool recordMenuItemEnabled;
		TreeView *projectTreeView;					//tree view control for the project pane
		ProjectPaneDataArray *projectPaneData;		//data class that controls the project pane
		System::Windows::Forms::ContextMenu *contextMenu;
		ItemAddedDelegate *ItemAddedHandler;		
		ItemDeletedDelegate *ItemDeletedHandler;
		ItemModifiedDelegate *ItemModifiedHandler;
		PerThreadChangedDelegate *PerThreadChangedHandler;
		CloseProjectUIDelegate *CloseProjectUIHandler;
		FocusChangedDelegate *FocusChangedHandler;
		RecordedSessionReplayStartedDelegate *RecordedSessionReplayStartedHandler;
		RecordedSessionReplayStoppedDelegate *RecordedSessionReplayStoppedHandler;
		RecordedSessionStartedDelegate *RecordedSessionStartedHandler;
		RecordedSessionStoppedDelegate *RecordedSessionStoppedHandler;
		ProcessRemovedDelegate *ProcessRemovedHandler;
		Hashtable *terminatedProcessTable;
		Hashtable *pausedProcessTable;
		MenuItem *pauseResumeMenuItem;
		MenuItem *stopMenuItem;

		void contextMenuCreateRecordedSession(Object *sender, EventArgs *args);
		void contextMenuCreateReport(Object *sender, EventArgs *args);
		void contextMenuCreateTest(Object *sender, EventArgs *args);
		void contextMenuCreateFault(Object *sender, EventArgs *args);
		void contextMenuCreateResourceTest(Object *sender, EventArgs *args);
		void contextMenuCreateNetworkCorruption(Object *sender, EventArgs *args);
		void contextMenuModifyNetworkCorruption(Object *sender, EventArgs *args);
		void contextMenuCreateFileCorruption(Object *sender, EventArgs *args);
		void contextMenuModifyFileCorruption(Object *sender, EventArgs *args);
		void contextMenuDelete(Object *sender, EventArgs *args);
		void contextMenuRename(Object *sender, EventArgs *args);
		void contextMenuModifyTest(Object *sender, EventArgs *args);
		void contextMenuModifyResourceTest(Object *sender, EventArgs *args);
		void saveProject(Object *sender, EventArgs *args);
		void PauseProcess(Object *sender, EventArgs *args);
		void TerminateProcess(Object *sender, EventArgs *args);
		void RestartProcess(Object *sender, EventArgs *args);
		void RemoveProcess(Object *sender, EventArgs *args);
		void contextMenuSave(Object *sender, EventArgs *args);
		void contextMenuReplay(Object *sender, EventArgs *args);
		void LayoutChanged(Object *sender, LayoutEventArgs *e);
		void OnMouseUp(Object *sender, MouseEventArgs *e);
		void OnKeyDown(Object *sender, KeyEventArgs *e);		
		void OnSelect(Object *sender, TreeViewEventArgs *e);
		void OnBeforeEdit(Object *sender, NodeLabelEditEventArgs *e);
		void OnAfterEdit(Object *sender, NodeLabelEditEventArgs *e);
		void OnDoubleClick(Object *sender, EventArgs *e);

		void OnItemAdded(DisplayableDataNode *Node);
		void OnItemDeleted(DisplayableDataNode *Node);
		void OnItemModified(DisplayableDataNode *dataNode, String *Name);
		void OnPerThreadChanged(bool perThread);
		void OnCloseProjectUI();
		void OnFocusChanged(DisplayableDataNode *dataNode);
		void RecordedSessionReplayStarted(DisplayableDataNode *dataNode);
		void RecordedSessionReplayStopped(DisplayableDataNode *dataNode);
		void RecordedSessionStarted(DisplayableDataNode *dataNode);
		void RecordedSessionStopped(DisplayableDataNode *dataNode);

		ArrayList *GetNode(NodeType Type, int processID, int threadID, TreeNode *startNode);
		ArrayList *GetNodeHelper(NodeType Type, int processID, int threadID, TreeNode *startNode);
		void AddNode(DisplayableDataNode *Node, int position);
		void onAddNode(DisplayableDataNode *dataNode, int position);
		void onDeleteNode(DisplayableDataNode *dataNode);
		void onProcessRemoved(unsigned int processID);
		bool InsertNode(DisplayableDataNode *Node, NodeType nodeType, int position);
		bool DeleteDataNodes(TreeNode *treeNode);
		bool IsNewThread(int processID, int threadID);
		bool IsNewProcess(int processID);
		void OnProcessTerminated(unsigned int processID);
		void OnProcessPaused(unsigned int processID);
		void OnProcessResumed(unsigned int processID);
	};
}