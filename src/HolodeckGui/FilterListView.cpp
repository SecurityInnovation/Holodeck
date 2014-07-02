//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FilterListView.cpp
//
// DESCRIPTION: Contains implementation for the class FilterListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 08 Apr 2003		 A. Kakrania	File created.
//*************************************************************************
#include "FilterListView.h"
#include "defines.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FilterListView
	// Description: Constructor for the FilterListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FilterListView::FilterListView()
	{
		this->View = System::Windows::Forms::View::Details;
		this->ColumnClick += new ColumnClickEventHandler (this, OnHeaderClick);
		this->menuitem_CustomFilterHandler = new EventHandler (this, localFilterFunc);
		this->menuitem_SortHandler = new EventHandler (this, localColumnSortFunc);

		headerMenu = new System::Windows::Forms::ContextMenu ();
		customFilterMenuItem = new MenuItem (new String ("Custom Filter..."), this->menuitem_CustomFilterHandler);
		sortMenuItem = new MenuItem (new String ("Sort column"), this->menuitem_SortHandler); 
		headerMenu->MenuItems->Add (customFilterMenuItem);
		headerMenu->MenuItems->Add (new String ("-"));
		headerMenu->MenuItems->Add (sortMenuItem);

	}

	//*************************************************************************
	// Method:		~FilterListView
	// Description: Destructor for the FilterListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FilterListView::~FilterListView()
	{
	}

	//*************************************************************************
	// Method:		WndProc
	// Description: Handles messages before they are sent to the ListView
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FilterListView::WndProc(Message *msg)
	{
		VirtualListView::WndProc(msg);
	}

	//*************************************************************************
	// Method:		AddColumnHeader
	// Description: Adds the supplied ColumnHeader to the listview and associates it
	//				with the given custom filter handler in a hash table
	//
	// Parameters:
	//		newHeader			- the column header to be added to the listview
	//		customFilterHandler	- the filter handler function for this column, can be NULL
	//
	// Return Value: None
	//*************************************************************************
	void FilterListView::AddColumnHeader (ColumnHeader * newHeader, EventHandler * customFilterHandler)
	{
		if (this->customFilters == NULL)
			this->customFilters = new Collections::Hashtable();

		this->customFilters->Add (newHeader, customFilterHandler);
		Columns->Add (newHeader);
	}

	//*************************************************************************
	// Method:		localFilterFunc
	// Description: The local function which is called when "Custom Filter .." 
	//				is selected. This method calls the appropriate handler
	//				function (if exists) by looking it up in the hash table.
	//
	// Parameters:
	//		sender	- the object which sent this 
	//		e		- the event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FilterListView::localFilterFunc(System::Object * sender, EventArgs * e)
	{
		if (customFilters->Item[columnWithFocus] != NULL)
		{
			//checkItemRadio (static_cast <MenuItem *> (sender));
			EventHandler * customFilterMethod = static_cast <EventHandler *> (customFilters->Item[columnWithFocus]);
			customFilterMethod->Invoke (sender, EventArgs->Empty);
		}
	}

	//*************************************************************************
	// Method:		OnHeaderClick
	// Description: This method is called when a column header is clicked
	//				It displays the context menu
	//
	// Parameters:
	//		sender	- the object which sent this 
	//		e		- the event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FilterListView::OnHeaderClick (System::Object * sender, ColumnClickEventArgs * e)
	{
		//calculate the display position for the context menu
		int displayPos = 0;
		ListView * lv = static_cast <ListView *> (sender);
		for (int i=0; i < e->Column; i++)
			displayPos += lv->Columns->Item[i]->Width;
		displayPos += (int) (lv->Columns->Item[e->Column]->Width * 12 / 100);

		// save a reference to the column with focus
		columnWithFocus = lv->Columns->Item[e->Column];

		// check if this column has a custom filter type defined
		if (customFilters->Item[columnWithFocus] == NULL)
			headerMenu->MenuItems->Item[customFilterMenuItem->Index]->Enabled = false;
		else
			headerMenu->MenuItems->Item[customFilterMenuItem->Index]->Enabled = true;

		// check if a sort method is defined
		if (this->ColumnSortEventHandler)
			headerMenu->MenuItems->Item[sortMenuItem->Index]->Enabled = true;
		else
			headerMenu->MenuItems->Item[sortMenuItem->Index]->Enabled = false;

		// show the context menu
		headerMenu->Show (lv, Drawing::Point (displayPos, 10));
	}

	//*************************************************************************
	// Method:		localColumnSortFunc
	// Description: This method is called when "Sort" is selected from the context
	//				menu. It invokes the defined sorting method, if one exists
	//
	// Parameters:
	//		sender	- the object which sent this 
	//		e		- the event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FilterListView::localColumnSortFunc(System::Object * sender, EventArgs * e)
	{
		if (ColumnSortEventHandler != NULL)
		{
			ColumnSortEventHandler->Invoke (sender, new ColumnClickEventArgs (this->Columns->IndexOf (columnWithFocus)));
		}
	}
}