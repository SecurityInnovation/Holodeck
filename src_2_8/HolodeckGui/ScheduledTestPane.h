//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestPane.h
//
// DESCRIPTION: Contains definition for the class ScheduledTestPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "ScheduledTestDataArray.h"
#include "UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <SiControls.dll>

using namespace UIData;
using namespace System;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace FunctionDatabase;
using namespace SIControls;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents a log view pane
	//*************************************************************************
	public __gc class ScheduledTestPane : public SelectableFieldPane
	{
	protected:
		UIEventRegistry * uiEventRegistry;
		ScheduledTestDataArray *dataArray;
		CheckBoxListView *listView;
		ArrayList *allColumns;
		ColumnHeader *onOffHeader;
		ColumnHeader *targetThreadHeader;
		ColumnHeader *functionNameHeader;
		ColumnHeader *returnValueHeader;
		ColumnHeader *errorCodeHeader;
		ColumnHeader *matchParamHeader;
		ColumnHeader *changeParamHeader;
		ColumnHeader *testOwnerHeader;
		ColumnHeader *pauseAfterTestHeader;
		ColumnHeader *fireConditionHeader;

		ArrayList *allColumnWidths;
		ItemCheckEventHandler *listView_ItemCheckEventHandler;

		bool catchCheckEvent;
		bool perThreadMode;
		int threadID;

		ItemAddedDelegate *addedDelegate;
		ItemDeletedDelegate *deletedDelegate;
		void onItemAdded(DisplayableDataNode *node);
		void onItemDeleted(DisplayableDataNode *node);

		void Dispose(bool disposing);
		void AddListViewItemString(ListViewItem *item, String *text, bool asSubItem);
		void UpdateListViewItemStrings(ListViewItem *item);
		void AddStringsToListViewItem(ListViewItem *item, ScheduledTestDataNode *node);
		ListViewItem *ConvertScheduledTestToListViewItem(ScheduledTestDataNode *node);
		void DeleteTest(Object *sender, EventArgs *e);
		void CreateTest(Object *sender, EventArgs *e);
		void ModifyTest(Object *sender, EventArgs *e);
		void onDoubleClick(Object *sender, EventArgs *e);
		void onKeyDown(Object* sender, KeyEventArgs *e);
		void onMouseUp(Object* sender, MouseEventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void listView_ItemCheckEvent(Object *sender, ItemCheckEventArgs * e);

	public:
		ScheduledTestPane(ScheduledTestDataArray *dataArray, unsigned int threadID);
		~ScheduledTestPane();

		[Browsable(false)]
		__property int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property int get_ThreadID() { return threadID; }
		[Browsable(false)]
		__property String *get_PaneName() { return "ScheduledTestPane"; }
		[Browsable(false)]
		__property ListView *get_PaneListView() { return listView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }
		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns();
		__property virtual void set_VisibleColumns(ArrayList *headers);
		[Browsable(false)]
		__property ScheduledTestDataArray *get_DataArray() { return dataArray; }

	};
}