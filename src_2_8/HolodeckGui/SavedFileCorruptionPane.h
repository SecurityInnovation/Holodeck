//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionPane.h
//
// DESCRIPTION: Contains definition for the class SavedFileCorruptionPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "SelectableFieldPane.h"
#include "SavedFileCorruptionDataArray.h"
#include "FilterListView.h"
#include "FieldChooser.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>

using namespace UIData;
using namespace System;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents a saved file corruption pane
	//*************************************************************************
	public __gc class SavedFileCorruptionPane : public SelectableFieldPane
	{
	protected:
		SavedFileCorruptionDataArray *dataArray;
		ListView *listView;
		ArrayList *allColumns;
		ToolTip *toolTip;

		ColumnHeader *origFileHeader;
		ColumnHeader *corruptedFileHeader;

		ItemAddedDelegate *addedDelegate;
		ItemDeletedDelegate *deletedDelegate;
		void onItemAdded(DisplayableDataNode *node);
		void onItemDeleted(DisplayableDataNode *node);

		void Dispose(bool disposing);
		void AddListViewItemString(ListViewItem *item, String *text, bool asSubItem);
		ListViewItem *ConvertSavedFileCorruptionToListViewItem(SavedFileCorruptionDataNode *node);
		void onKeyDown(Object* sender, KeyEventArgs *e);
		void onMouseUp(Object* sender, MouseEventArgs *e);
		void onDoubleClick(Object *sender, EventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *args);
		void onShowFieldChooser(Object *sender, EventArgs *args);

		void SaveOriginalFile(Object *sender, EventArgs *e);
		void SaveCorruptedFile(Object *sender, EventArgs *e);
		void SaveRTFDetails(Object *sender, EventArgs *e);
		void DeleteCorruptedFile(Object *sender, EventArgs *e);

	public:
		SavedFileCorruptionPane(SavedFileCorruptionDataArray *dataArray);
		~SavedFileCorruptionPane();

		[Browsable(false)]
		__property SavedFileCorruptionDataArray *get_DataArray() { return dataArray; }
		[Browsable(false)]
		__property int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property String *get_PaneName() { return "SavedFileCorruptionPane"; }

		[Browsable(false)]
		__property ListView *get_PaneListView() { return listView; }
		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }

		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns();
		__property virtual void set_VisibleColumns(ArrayList *headers);
	};
}