//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FilterTreeListView.cpp
//
// DESCRIPTION: Contains implementation for the class FilterTreeListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------  -----------------------------------------
// 18 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "FilterTreeListView.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		FilterTreeListView
	// Description: Constructor for the FilterTreeListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FilterTreeListView::FilterTreeListView()
	{
		this->ColumnClick += new ColumnClickEventHandler(this, OnHeaderClick);
		this->HeaderMenuEvent += new HeaderMenuEventHandler(this, OnHeaderMenuEvent);
		this->menuItemCustomFilterHandler = new EventHandler(this, LocalFilterFunc);
		this->menuItemSortHandler = new EventHandler(this, LocalColumnSortFunc);

		headerMenu = new System::Windows::Forms::ContextMenu();
		customFilterMenuItem = new MenuItem("Custom Filter...", menuItemCustomFilterHandler);
		sortMenuItem = new MenuItem("Sort column", menuItemSortHandler);
		headerMenu->MenuItems->Add(customFilterMenuItem);
		headerMenu->MenuItems->Add("-");
		headerMenu->MenuItems->Add(sortMenuItem);
	}

	//*************************************************************************
	// Method:		~FilterTreeListView
	// Description: Destructor for the FilterTreeListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FilterTreeListView::~FilterTreeListView()
	{
	}

	//*************************************************************************
	// Method:		OnHeaderMenuEvent
	// Description: called when the header menu is to be displayed
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FilterTreeListView::OnHeaderMenuEvent(Object *sender, MouseEventArgs *e)
	{
		int columnToUse = 0;
		int widthSoFar = 0;
		for (columnToUse = 0; columnToUse < Columns->Count; columnToUse++)
		{
			int thisColWidth = Columns->Item[columnToUse]->Width;
			if (((e->X + hScrollBar->Value) > widthSoFar) && ((e->X + hScrollBar->Value) < (widthSoFar + thisColWidth)))
			{
				break;
			}
			widthSoFar += thisColWidth;
		}

		if (columnToUse >= Columns->Count)
			return;

		columnWithFocus = Columns->Item[columnToUse];

		// check if this column has a custom filter type defined
		if (!customFilters->Item[columnWithFocus])
			headerMenu->MenuItems->Item[customFilterMenuItem->Index]->Enabled = false;
		else
			headerMenu->MenuItems->Item[customFilterMenuItem->Index]->Enabled = true;

		// check if a sort method is defined
		if (this->ColumnSortEventHandler)
			headerMenu->MenuItems->Item[sortMenuItem->Index]->Enabled = true;
		else
			headerMenu->MenuItems->Item[sortMenuItem->Index]->Enabled = false;

		// show the context menu
		headerMenu->Show (this, Drawing::Point (e->X, 10));
	}

	//*************************************************************************
	// Method:		AddColumnHeader
	// Description: adds a header to the tree view
	//
	// Parameters:
	//	newHeader - the header to add
	//	customFilterHandler - the delegate handler to call on click
	//
	// Return Value: None
	//*************************************************************************
	void FilterTreeListView::AddColumnHeader(ToggleColumnHeader *newHeader, EventHandler *customFilterHandler)
	{
		if (!customFilters)
			customFilters = new Hashtable();

		customFilters->Add(newHeader, customFilterHandler);
		Columns->Add(newHeader);
	}

	//*************************************************************************
	// Method:		OnHeaderClick
	// Description: adds a header to the tree view
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FilterTreeListView::OnHeaderClick(Object *sender, ColumnClickEventArgs* e)
	{
		columnWithFocus = Columns->Item[e->Column];
		if (this->ColumnSortEventHandler)
			LocalColumnSortFunc(sender, EventArgs::Empty);
	}

	//*************************************************************************
	// Method:		LocalFilterFunc
	// Description: handles the filter event
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FilterTreeListView::LocalFilterFunc(Object *sender, EventArgs *e)
	{
		if (customFilters->Item[columnWithFocus])
		{
			EventHandler *customFilterMethod = static_cast<EventHandler *>(customFilters->Item[columnWithFocus]);
			customFilterMethod->Invoke(sender, EventArgs->Empty);
		}
	}

	//*************************************************************************
	// Method:		LocalColumnSortFunc
	// Description: handles the sort event
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FilterTreeListView::LocalColumnSortFunc(Object *sender, EventArgs *e)
	{
		if (ColumnSortEventHandler)
		{
			ColumnSortEventHandler->Invoke(sender, new ColumnClickEventArgs(Columns->IndexOf(columnWithFocus)));
		}
	}
}