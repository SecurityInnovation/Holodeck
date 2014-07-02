//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPane.h
//
// DESCRIPTION: Contains definition for the class LogPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "LogPaneDataArray.h"
#include "LogTreeListView.h"
#include "UIEventRegistry.h"
#include "DataEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>
#using <FunctionDatabase.dll>
#using <SIControls.dll>

using namespace UIData;
using namespace System;
using namespace GuiSupport;
using namespace FunctionDatabase;
using namespace SIControls;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents a log view pane
	//*************************************************************************
	public __gc class LogPane : public SelectableFieldPane
	{
	public:
		LogPane(LogPaneDataArray *data);
		~LogPane();

		LogTreeListView * get_PaneLogListView();

		[Browsable(false)]
		__property TreeListView *get_PaneTreeListView() { return treeListView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return treeListView->AllColumns; }
		[Browsable(false)]
		__property virtual String *get_PaneName() { return "LogPane"; }
		[Browsable(false)]
		__property ArrayList *get_VisibleColumns();
		__property void set_VisibleColumns(ArrayList *headers);
		__property int get_ProcessID() { return dataArray->ProcessID; }
		__property int get_ThreadID() { return dataArray->ThreadID; }
		__property int get_LogCount() { return  treeListView->ItemCount; }
		[Browsable(false)]
		__property ListViewItem *get_GetListViewItem(int i)
		{
			TreeListNode * tln = onTreeListViewQueryItem(i);
			if (tln)
				return tln->GetListViewItem();
			else
				return NULL;
		}

		[Browsable(false)]
		__property LogItem *get_GetLogItem(unsigned int i) { return this->dataArray->GetLogItemAt(i);}
		[Browsable(false)]
		__property SIControls::ColumnHeaderCollection * get_ColumnHeaders() { return treeListView->Columns;}
		[Browsable(false)]
		__property String *get_Title()
		{
			String * retString = "Executable: ";
			retString = String::Concat (retString, dataArray->FullExecutableName);
			retString = String::Concat (retString, new String("   ProcessID: "));
			retString = String::Concat (retString, dataArray->ProcessID.ToString());
			return retString;
		}
		[Browsable(false)]
		__property String *get_PrintTitle()
		{
			String * retString = "API Log: ";
			retString = String::Concat (retString, Path::GetFileName (dataArray->FullExecutableName));
			retString = String::Concat (retString, S"(PID:", dataArray->ProcessID.ToString()->Trim(), S")");
			return retString;
		}
		[Browsable(false)]
		__property LogPaneDataArray *get_DataArray() { return dataArray; }
		[Browsable(false)]
		__property unsigned int get_SelectedTreeListViewIndex()
		{
			if (this->treeListView->SelectedIndices->Count > 0)
				return *dynamic_cast<__box int *>(this->treeListView->SelectedIndices->get_Item(0));
			else
				return -1;
		}
		__property void set_SelectedTreeListViewIndex(unsigned int iSelected)
		{
			treeListView->SelectedIndex = (int) iSelected;
		}

		TreeListNode *onTreeListViewQueryItem(unsigned int index);
		TreeListNode *onTreeListViewQueryRow(unsigned int row);
		void onTreeListViewUnselectAll();
		static DateTime ConvertTimeStampToDateTime(String * timeStamp);

	private:
		UIEventRegistry * uiEventRegistry;
		DataEventRegistry *dataEventRegistry;
		LogPaneDataArray *dataArray;
		LogTreeListView *treeListView;
		StatusBarPanel *descriptionPanel;
		StatusBarPanel *activeOrSavedPanel;
		StatusBarPanel *countPanel;
		StatusBarPanel *visibleCountPanel;
		Timer *statusUpdateTimer;
		EventHandler *statusTickHandler;
		ProcessTerminatedDelegate *ProcessTerminatedHandler;
		ProcessPausedDelegate *ProcessPausedHandler;
		ProcessResumedDelegate *ProcessResumedHandler;
		SelectLogItemDelegate *SelectLogItemHandler;
		ErrorCodeDB *errorCodeDB;
		PerThreadChangedDelegate *PerThreadChangedHandler;
		CloseProjectUIDelegate *CloseProjectUIHandler;
		AutoFilterDelegate *FilterLogItemHandler;
		LogEntrySetReceivedDelegate *LogEntrySetReceivedHandler;

		ArrayList *allColumnWidths;

		unsigned long numVisibleEntries;

		void InitializeUI();
		void CleanUp();

		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onStatusUpdateTimerTick(Object *sender, EventArgs *args);
		void onColumnSort(Object *sender, ColumnClickEventArgs *args);
		void onListViewMouseUp(Object *sender, MouseEventArgs *args);
		void onListViewDoubleClick(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);
		void onFilterUpdate();
		void onLogEntrySetReceived(unsigned int numEntriesInSet);
		void onFilterNodeColumns(TreeListNode *node);
		void onProcessTerminated(unsigned int processID);
		void onProcessPaused(unsigned int processID);
		void onProcessResumed(unsigned int processID);
		void onExpandAll(Object *sender, EventArgs *args);
		void onCollapseAll(Object *sender, EventArgs *args);
		void onCreateScheduledTest(Object *sender, EventArgs *args);
		void onSelectLogItem(int pid, unsigned long filePosition);
		void onCloseProjectUI();
		void onExportToFile(Object *sender, EventArgs *args);
		
		bool matchString(String * string1, String * string2, String * matchType);
		bool FilterLogItem(LogItem * item);
		ListViewItem *ConvertLogItemToListViewItem(LogItem *item);
	};
}