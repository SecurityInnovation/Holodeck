//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ColumnHeaderCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class ColumnHeaderCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ColumnHeaderCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ColumnHeaderCollection
	// Description: Constructor for the ColumnHeaderCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ColumnHeaderCollection::ColumnHeaderCollection()
	{
	}

	//*************************************************************************
	// Method:		~ColumnHeaderCollection
	// Description: Destructor for the ColumnHeaderCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ColumnHeaderCollection::~ColumnHeaderCollection()
	{
	}

	//*************************************************************************
	// Method:		OnWidthResized
	// Description: Call to invoke the WidthResized delegate
	//
	// Parameters:
	//	sender - the sender of the resized event
	//	e - the args representing the resize event
	//
	// Return Value: None
	//*************************************************************************
	void ColumnHeaderCollection::OnWidthResized(Object *sender, EventArgs *e)
	{
		if (WidthResized)
			WidthResized->Invoke(sender, e);
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: returns the ToggleColumnHeader object at specified index
	//
	// Parameters:
	//	index - the index of the header to return
	//
	// Return Value: the ToggleColumnHeader object at the specified index
	//*************************************************************************
	ToggleColumnHeader *ColumnHeaderCollection::get_Item(int index)
	{
		ToggleColumnHeader *tch = new ToggleColumnHeader();
		try
		{
			tch = dynamic_cast<ToggleColumnHeader *>(List->get_Item(index));
		}
		catch(Exception *)
		{
		}
		return tch;
	}

	//*************************************************************************
	// Method:		set_Item
	// Description: sets the ToggleColumnHeader object at the specified index
	//
	// Parameters:
	//	index - the index to set the header at
	//	value - the new header value
	//
	// Return Value: None
	//*************************************************************************
	void ColumnHeaderCollection::set_Item(int index, ToggleColumnHeader *value)
	{
		value->WidthResized += new EventHandler(this, OnWidthResized);
		List->set_Item(index, value);
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a ToggleColumnHeader to the collection
	//
	// Parameters:
	//	item - the header to add
	//
	// Return Value: the index the header was added at
	//*************************************************************************
	int ColumnHeaderCollection::Add(ToggleColumnHeader *item)
	{
		item->WidthResized += new EventHandler(this, OnWidthResized);
		item->Index = List->Add(item);
		return item->Index;
	}

	//*************************************************************************
	// Method:		Add
	// Description: Creates a new ToggleColumnHeader and adds it to the collection
	//
	// Parameters:
	//	str - the text for the column header
	//	width - the width of the header
	//	textAlign - the horizontal text alignment of the header
	//
	// Return Value: the newly created and added ToggleColumnHeader object
	//*************************************************************************
	ToggleColumnHeader *ColumnHeaderCollection::Add(String *str, int width, HorizontalAlignment textAlign)
	{
		ToggleColumnHeader *tch = new ToggleColumnHeader();
		tch->Text = str;
		tch->Width = width;
		tch->TextAlign = textAlign;
		tch->WidthResized += new EventHandler(this, OnWidthResized);

		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			tch->Index = List->Add(tch);
		}
		catch(Exception *)
		{
		}
		__finally
		{
			System::Threading::Monitor::Exit(List->SyncRoot);
		}
		return tch;
	}

	//*************************************************************************
	// Method:		AddRange
	// Description: Adds an array of column headers to the collection
	//
	// Parameters:
	//	items - the array of column headers to add
	//
	// Return Value: None
	//*************************************************************************
	void ColumnHeaderCollection::AddRange(ToggleColumnHeader *items[])
	{
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			for (int i = 0; i < items->Length; i++)
			{
				ToggleColumnHeader *thisItem = dynamic_cast<ToggleColumnHeader *>(items->get_Item(i));
				thisItem->WidthResized += new EventHandler(this, OnWidthResized);
				thisItem->Index = List->Add(thisItem);
			}
		}
		catch(Exception *)
		{
		}
		__finally
		{
			System::Threading::Monitor::Exit(List->SyncRoot);
		}
	}
}