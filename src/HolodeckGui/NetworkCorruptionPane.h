//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionPane.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "NetworkCorruptionDataArray.h"
#include "FilterListView.h"
#include "UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <SIControls.dll>

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
	// class that all represents a network corruption pane
	//*************************************************************************
	public __gc class NetworkCorruptionPane : public SelectableFieldPane
	{
	protected:
		UIEventRegistry * uiEventRegistry;
		NetworkCorruptionDataArray *dataArray;
		CheckBoxListView *listView;
		ArrayList *allColumns;

		ArrayList *allColumnWidths;
		ItemCheckEventHandler *listView_ItemCheckEventHandler;

		ColumnHeader *onOffHeader;
		ColumnHeader *portHeader;
		ColumnHeader *typeHeader;
		ColumnHeader *searchForHeader;
		ColumnHeader *replaceWithHeader;

		bool catchCheckEvent;

		ItemAddedDelegate *addedDelegate;
		ItemDeletedDelegate *deletedDelegate;
		void onItemAdded(DisplayableDataNode *node);
		void onItemDeleted(DisplayableDataNode *node);

		void Dispose(bool disposing);
		void AddListViewItemString(ListViewItem *item, String *text, bool asSubItem);
		void UpdateListViewItemStrings(ListViewItem *item);
		void AddStringsToListViewItem(ListViewItem *item, NetworkCorruptionDataNode *node);
		ListViewItem *ConvertNetworkCorruptionToListViewItem(NetworkCorruptionDataNode *node);
		void onKeyDown(Object* sender, KeyEventArgs *e);
		void onMouseUp(Object* sender, MouseEventArgs *e);
		void onDoubleClick(Object *sender, EventArgs *e);
		void DeleteNetworkCorruptionFault(Object *sender, EventArgs *e);
		void CreateNetworkCorruptionFault(Object *sender, EventArgs *e);
		void ModifyNetworkCorruptionFault(Object *sender, EventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void listView_ItemCheckEvent(Object *sender, ItemCheckEventArgs * e);

	public:
		NetworkCorruptionPane(NetworkCorruptionDataArray *dataArray);
		~NetworkCorruptionPane();

		[Browsable(false)]
		__property NetworkCorruptionDataArray *get_DataArray() { return dataArray; }
		[Browsable(false)]
		__property int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property String *get_PaneName() { return "NetworkCorruptionPane"; }

		[Browsable(false)]
		__property ListView *get_PaneListView() { return listView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }

		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns();
		__property virtual void set_VisibleColumns(ArrayList *headers);
	};
}