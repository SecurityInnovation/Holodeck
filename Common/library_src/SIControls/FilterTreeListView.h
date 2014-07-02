//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FilterTreeListView.h
//
// DESCRIPTION: Contains definition for the class FilterTreeListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------  -----------------------------------------
// 18 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include "TreeListView.h"

namespace SIControls
{
	//*********************************************************************
	// A tree control where the nodes can be multi column list view items
	// and the rows can be filtered to match column values
	//*********************************************************************
	public __gc class FilterTreeListView : public TreeListView
	{
	public:
		FilterTreeListView();
		~FilterTreeListView();

		EventHandler *columnChangedEvent;
		ColumnClickEventHandler *ColumnSortEventHandler;

		void AddColumnHeader(ToggleColumnHeader *newHeader, EventHandler *customFilterHandler);
	protected:
		System::Windows::Forms::ContextMenu *headerMenu;
		ToggleColumnHeader *columnWithFocus;
		MenuItem *menuColumns;

		ArrayList *hiddenHeaders;
		Hashtable *customFilters;
		MenuItem *customFilterMenuItem;
		MenuItem *sortMenuItem;

		EventHandler *menuItemCustomFilterHandler;
		EventHandler *menuItemSortHandler;

		void OnHeaderMenuEvent(Object *sender, MouseEventArgs *e);
		void OnHeaderClick(Object *sender, ColumnClickEventArgs* e);
		void LocalFilterFunc(Object *sender, EventArgs *e);
		void LocalColumnSortFunc(Object *sender, EventArgs *e);
	};
}