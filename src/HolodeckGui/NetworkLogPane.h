//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPane.h
//
// DESCRIPTION: Contains definition for the class NetworkLogPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "NetworkLogPaneDataArray.h"
#include "NetworkLogListView.h"
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
	public __gc class NetworkLogPane : public SelectableFieldPane
	{
	public:
		NetworkLogPane(NetworkLogPaneDataArray *data);
		~NetworkLogPane();

		[Browsable(false)]
		__property ContainerListView *get_PaneContainerListView() { return logListView; }

		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return logListView->AllColumns; }

		[Browsable(false)]
		__property virtual String *get_PaneName() { return "NetworkLogPane"; }

		[Browsable(false)]
		__property ArrayList *get_VisibleColumns();
		__property void set_VisibleColumns(ArrayList *headers);
		__property int get_ProcessID() { return dataArray->ProcessID; }
		__property int get_ThreadID() { return dataArray->ThreadID; }
		__property int get_LogCount() { return  logListView->ItemCount; }
		[Browsable(false)]
		__property NetworkLogItem *get_LogItem(unsigned int i) { return this->dataArray->GetLogItemAt(i); }
		[Browsable(false)]
		__property SIControls::ColumnHeaderCollection * get_ColumnHeaders() { return logListView->Columns; }
		[Browsable(false)]
		__property String *get_Title()
		{
			String * retString = "Network Logs ";
			retString = String::Concat (retString, new String("   ProcessID: "));
			retString = String::Concat (retString, dataArray->ProcessID.ToString());
			return retString;
		}
		[Browsable(false)]
		__property NetworkLogPaneDataArray *get_DataArray() { return dataArray; }
		[Browsable(false)]
		__property unsigned int get_SelectedListViewIndex()
		{
			if (this->logListView->SelectedIndices->Count > 0)
				return *dynamic_cast<__box int *>(this->logListView->SelectedIndices->get_Item(0));
			else
				return -1;
		}
		[Browsable(false)]
		__property void set_SelectedListViewIndex(unsigned int iSelected)
		{
			logListView->SelectedIndex = (int) iSelected;
		}

		ContainerListViewItem *OnQueryContainerListViewItem(unsigned int index);
		
		[Browsable(false)]
		__property ListViewItem *get_GetListViewItem(unsigned int index){return OnQueryContainerListViewItem(index)->GetListViewItem();}

	private:
		UIEventRegistry * uiEventRegistry;
		DataEventRegistry *dataEventRegistry;
		NetworkLogPaneDataArray *dataArray;
		NetworkLogListView *logListView;
		StatusBarPanel *descriptionPanel;
		StatusBarPanel *activeOrSavedPanel;
		StatusBarPanel *countPanel;
		StatusBarPanel *visibleCountPanel;
		System::Threading::Timer *statusUpdateTimer;
		System::Threading::TimerCallback *statusTickHandler;
		PerThreadChangedDelegate *PerThreadChangedHandler;
		ToolTip *toolTip;

		unsigned long numVisibleEntries;

		ArrayList *allColumnWidths;

		void InitializeUI();

		void onListViewMouseUp(Object *sender, MouseEventArgs *args);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onStatusUpdateTimerTick(Object *state);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void onFilterItemColumns(ContainerListViewItem *item);
		void onFilterUpdate();
		void onLogEntrySetReceived(unsigned int numEntriesInSet);
		unsigned int OnQueryContainerListViewItemIndex(ContainerListViewItem *item);
		void OnSetListViewItemSelected(int index, bool selected);
		void OnSetListViewItemFocused(int index, bool focused);
		void onColumnSort(Object *sender, ColumnClickEventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);
		void onExportToFile(Object *sender, EventArgs *args);
		void onDoubleClick(Object *sender, EventArgs *e);

		bool FilterLogItem(NetworkLogItem *item);
	};
}