//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestPane.h
//
// DESCRIPTION: Contains definition for the class ResourceTestPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Dec 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "ResourceTestDataArray.h"
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
	// class that all represents a resource test pane
	//*************************************************************************
	public __gc class ResourceTestPane : public SelectableFieldPane
	{
	protected:
		UIEventRegistry * uiEventRegistry;
		ResourceTestDataArray *dataArray;
		CheckBoxListView *listView;
		ArrayList *allColumns;

		ColumnHeader *onOffHeader;
		ColumnHeader *resourceNameHeader;
		ColumnHeader *faultNameHeader;

		bool catchCheckEvent;

		ArrayList *allColumnWidths;
		ItemCheckEventHandler *listView_ItemCheckEventHandler;

		ItemAddedDelegate *addedDelegate;
		ItemDeletedDelegate *deletedDelegate;
		void onItemAdded(DisplayableDataNode *node);
		void onItemDeleted(DisplayableDataNode *node);

		void Dispose(bool disposing);
		void AddListViewItemString(ListViewItem *item, String *text, bool asSubItem);
		void UpdateListViewItemStrings(ListViewItem *item);
		void AddStringsToListViewItem(ListViewItem *item, ResourceTestDataNode *node);
		ListViewItem *ConvertResourceTestToListViewItem(ResourceTestDataNode *node);
		void onKeyDown(Object* sender, KeyEventArgs *e);
		void onMouseUp(Object* sender, MouseEventArgs *e);
		void onDoubleClick(Object *sender, EventArgs *e);
		void DeleteResourceTest(Object *sender, EventArgs *e);
		void CreateResourceTest(Object *sender, EventArgs *e);
		void ModifyResourceTest(Object *sender, EventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void listView_ItemCheckEvent(Object *sender, ItemCheckEventArgs * e);

	public:
		ResourceTestPane(ResourceTestDataArray *dataArray);
		~ResourceTestPane();

		[Browsable(false)]
		__property ResourceTestDataArray *get_DataArray() { return dataArray; }
		[Browsable(false)]
		__property int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property String *get_PaneName() { return "ResourceTestPane"; }

		[Browsable(false)]
		__property ListView *get_PaneListView() { return listView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }

		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns();
		__property virtual void set_VisibleColumns(ArrayList *headers);
	};
}