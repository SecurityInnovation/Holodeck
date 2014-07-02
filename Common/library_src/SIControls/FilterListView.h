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
// -----------  ---  -----------  -----------------------------------------
// 29 Jan 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include "ContainerListView.h"

namespace SIControls
{
	//*********************************************************************
	// A list control where the nodes can be multi column list view items
	// and the rows can be filtered to match column values
	//*********************************************************************
	public __gc class FilterListView : public ContainerListView
	{
	public:
		FilterListView();
		~FilterListView();

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
		virtual bool EnableColumnSorting(ToggleColumnHeader *column) { return true; }
	};
}