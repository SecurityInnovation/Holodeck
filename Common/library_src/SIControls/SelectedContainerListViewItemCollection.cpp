//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectedContainerListViewItemCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class
//				SelectedContainerListViewItemCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "SelectedContainerListViewItemCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		get_Item
	// Description: returns the ContainerListViewItem at the specified index
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: the ContainerListViewItem at the specified index
	//*************************************************************************
	ContainerListViewItem *SelectedContainerListViewItemCollection::get_Item(int index)
	{
		return dynamic_cast<ContainerListViewItem *>(List->get_Item(index));
	}

	//*************************************************************************
	// Method:		set_Item
	// Description: sets the ContainerListViewItem at the specified index
	//
	// Parameters:
	//	index - the index of the item to set
	//	value - the new value to set at the index
	//
	// Return Value: None
	//*************************************************************************
	void SelectedContainerListViewItemCollection::set_Item(int index, ContainerListViewItem *value)
	{
		List->set_Item(index, value);
	}

	//*************************************************************************
	// Method:		IndexOf
	// Description: returns the index of the specified item in the list
	//
	// Parameters:
	//	item - the item to get the index of
	//
	// Return Value: the index of the specified item
	//*************************************************************************
	int SelectedContainerListViewItemCollection::IndexOf(ContainerListViewItem *item)
	{
		return List->IndexOf(item);
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a list view item to the collection
	//
	// Parameters:
	//	item - the item to add
	//
	// Return Value: the index the item was added at
	//*************************************************************************
	int SelectedContainerListViewItemCollection::Add(ContainerListViewItem *item)
	{
		item->Index = List->Add(item);
		return item->Index;
	}

	//*************************************************************************
	// Method:		AddRange
	// Description: Adds a set of list view items to the collection
	//
	// Parameters:
	//	items - the set of items to add
	//
	// Return Value: None
	//*************************************************************************
	void SelectedContainerListViewItemCollection::AddRange(ContainerListViewItem *items[])
	{
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			for (int i = 0; i < items->Length; i++)
			{
				ContainerListViewItem *thisItem = dynamic_cast<ContainerListViewItem *>(items->get_Item(i));
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

	//*************************************************************************
	// Method:		Remove
	// Description: removes an item from the collection
	//
	// Parameters:
	//	item - the item to remove
	//
	// Return Value: None
	//*************************************************************************
	void SelectedContainerListViewItemCollection::Remove(ContainerListViewItem *item)
	{
		List->Remove(item);
	}

	//*************************************************************************
	// Method:		ClearAll
	// Description: removes all items from the collection
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SelectedContainerListViewItemCollection::ClearAll()
	{
		List->Clear();
	}
}