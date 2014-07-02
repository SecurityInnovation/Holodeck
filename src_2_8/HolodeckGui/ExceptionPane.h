//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionPane.h
//
// DESCRIPTION: Contains definition for the class ExceptionPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "ExceptionDataArray.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace UIData;
using namespace System;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::IO;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents an exception pane
	//*************************************************************************
	public __gc class ExceptionPane : public SelectableFieldPane
	{
	protected:
		ExceptionDataArray *dataArray;
		ListView *listView;
		ArrayList *allColumns;
		ArrayList *allColumnWidths;
		ColumnHeader *timeStampHeader;
		ColumnHeader *exeNameHeader;
		ColumnHeader *targetThreadHeader;
		ColumnHeader *exceptionHeader;
		ColumnHeader *miniDumpHeader;

		bool perThreadMode;
		int threadID;

		ItemAddedDelegate *addedDelegate;
		ItemDeletedDelegate *deletedDelegate;
		void onItemAdded(DisplayableDataNode *node);
		void onItemDeleted(DisplayableDataNode *node);

		void Dispose(bool disposing);
		void AddListViewItemString(ListViewItem *item, String *text, bool asSubItem);
		void UpdateListViewItemStrings(ListViewItem *item);
		void AddStringsToListViewItem(ListViewItem *item, ExceptionDataNode *node);
		ListViewItem *ConvertExceptionToListViewItem(ExceptionDataNode *node);
		void DeleteException(Object *sender, EventArgs *e);
		void SaveException(Object* sender, EventArgs *e);
		void onDoubleClick(Object *sender, EventArgs *e);
		void onKeyDown(Object* sender, KeyEventArgs *e);
		void onMouseUp(Object* sender, MouseEventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);
		void onCopyToClipboard(Object *sender, EventArgs *args);

	public:
		ExceptionPane(ExceptionDataArray *dataArray, unsigned int threadID);
		~ExceptionPane();

		[Browsable(false)]
		__property int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property int get_ThreadID() { return threadID; }
		[Browsable(false)]
		__property String *get_PaneName() { return "ExceptionPane"; }
		[Browsable(false)]
		__property ListView *get_PaneListView() { return listView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }
		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns();
		__property virtual void set_VisibleColumns(ArrayList *headers);
		[Browsable(false)]
		__property ExceptionDataArray *get_DataArray() { return dataArray; }

	};
}