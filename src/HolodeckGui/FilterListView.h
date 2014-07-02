//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FilterListView.h
//
// DESCRIPTION: Contains definition for the class FilterListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 08 Apr 2003		 A. Kakrania	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>

#include <stdio.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	public __gc class FilterListView: public GuiSupport::VirtualListView
	{
	public:
		FilterListView();
		~FilterListView();
		void WndProc(Message* msg);

		EventHandler * columnsChangedEvent;
		ColumnClickEventHandler * ColumnSortEventHandler;

		void AddColumnHeader (ColumnHeader * newHeader, EventHandler * CustomFilterHandler);

	private:
		System::Windows::Forms::ContextMenu * headerMenu;
		System::Windows::Forms::ColumnHeader * columnWithFocus;
		MenuItem * menuColumns;
		Collections::ArrayList * hiddenHeaders;
		Collections::Hashtable * customFilters;
		MenuItem * customFilterMenuItem;
		MenuItem * sortMenuItem;

		EventHandler * menuitem_CustomFilterHandler;
		EventHandler * menuitem_SortHandler;

		void OnHeaderClick (System::Object * sender, ColumnClickEventArgs * e);
		void localFilterFunc(System::Object * sender, EventArgs * e);
		void localColumnSortFunc(System::Object * sender, EventArgs * e);
	};
}