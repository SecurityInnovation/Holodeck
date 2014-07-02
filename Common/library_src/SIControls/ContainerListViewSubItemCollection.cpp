//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewSubItemCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class
//				ContainerListViewSubItemCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ContainerListViewSubItemCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		get_Item
	// Description: returns the ContainerListViewSubItem at the specified index
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: the ContainerListViewSubItem at the specified index
	//*************************************************************************
	ContainerListViewSubItem *ContainerListViewSubItemCollection::get_Item(int index)
	{
		try
		{
			return dynamic_cast<ContainerListViewSubItem *>(List->get_Item(index));
		}
		catch(Exception *)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		set_Item
	// Description: sets the ContainerListViewSubItem at the specified index
	//
	// Parameters:
	//	index - the index of the item to set
	//	value - the new value to set at the index
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItemCollection::set_Item(int index, ContainerListViewSubItem *value)
	{
		List->set_Item(index, value);
		OnItemsChanged(new ItemsChangedEventArgs(index));
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a list view sub item to the collection
	//
	// Parameters:
	//	item - the item to add
	//
	// Return Value: the index the item was added at
	//*************************************************************************
	int ContainerListViewSubItemCollection::Add(ContainerListViewSubItem *item)
	{
		int index = List->Add(item);
		OnItemsChanged(new ItemsChangedEventArgs(index));
		return index;
	}

	//*************************************************************************
	// Method:		Add
	// Description: Creates a list view sub item and adds it to the collection
	//
	// Parameters:
	//	control - the control to use to create the sub item
	//
	// Return Value: the added item
	//*************************************************************************
	ContainerListViewSubItem *ContainerListViewSubItemCollection::Add(Control *control)
	{
		ContainerListViewSubItem *thisItem = new ContainerListViewSubItem(control);
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			OnItemsChanged(new ItemsChangedEventArgs(List->Add(thisItem)));
		}
		catch(Exception *)
		{
		}
		__finally
		{
			System::Threading::Monitor::Exit(List->SyncRoot);
		}
		return thisItem;
	}

	//*************************************************************************
	// Method:		Add
	// Description: Creates a list view sub item and adds it to the collection
	//
	// Parameters:
	//	str - the text to use to create the new sub item
	//
	// Return Value: the added item
	//*************************************************************************
	ContainerListViewSubItem *ContainerListViewSubItemCollection::Add(String *str)
	{
		ContainerListViewSubItem *thisItem = new ContainerListViewSubItem(str);
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			OnItemsChanged(new ItemsChangedEventArgs(List->Add(thisItem)));
		}
		catch(Exception *)
		{
		}
		__finally
		{
			System::Threading::Monitor::Exit(List->SyncRoot);
		}
		return thisItem;
	}

	//*************************************************************************
	// Method:		AddRange
	// Description: Adds a set of list view sub items to the collection
	//
	// Parameters:
	//	items - the set of items to add
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItemCollection::AddRange(ContainerListViewSubItem *items[])
	{
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			for (int i = 0; i < items->Length; i++)
			{
				OnItemsChanged(new ItemsChangedEventArgs(List->Add(items->get_Item(i))));
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

	//*************************************************************************
	// Method:		OnItemsChanged
	// Description: Invokes the ItemsChanged delegate for this object
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItemCollection::OnItemsChanged(ItemsChangedEventArgs *e)
	{
		if (ItemsChanged)
			ItemsChanged->Invoke(this, e);
	}
}