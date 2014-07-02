//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectedTreeListNodeCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class SelectedTreeListNodeCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "SelectedTreeListNodeCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SelectedTreeListNodeCollection
	// Description: Constructor for the SelectedTreeListNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SelectedTreeListNodeCollection::SelectedTreeListNodeCollection()
	{
	}

	//*************************************************************************
	// Method:		~SelectedTreeListNodeCollection
	// Description: Destructor for the SelectedTreeListNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SelectedTreeListNodeCollection::~SelectedTreeListNodeCollection()
	{
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: returns the TreeListNode at the specified index
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: the TreeListNode at the specified index
	//*************************************************************************
	TreeListNode *SelectedTreeListNodeCollection::get_Item(int index)
	{
		return dynamic_cast<TreeListNode *>(List->get_Item(index));
	}

	//*************************************************************************
	// Method:		set_Item
	// Description: sets the TreeListNode at the specified index
	//
	// Parameters:
	//	index - the index of the item to set
	//	value - the new value to set at the index
	//
	// Return Value: None
	//*************************************************************************
	void SelectedTreeListNodeCollection::set_Item(int index, TreeListNode *value)
	{
		List->set_Item(index, value);
	}

	//*************************************************************************
	// Method:		IndexOf
	// Description: returns the index of the specified node in the list
	//
	// Parameters:
	//	item - the node to get the index of
	//
	// Return Value: the index of the specified node
	//*************************************************************************
	int SelectedTreeListNodeCollection::IndexOf(TreeListNode *item)
	{
		return List->IndexOf(item);
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a tree list node to the collection
	//
	// Parameters:
	//	item - the node to add
	//
	// Return Value: the index the node was added at
	//*************************************************************************
	int SelectedTreeListNodeCollection::Add(TreeListNode *item)
	{
		item->Index = List->Add(item);
		return item->Index;
	}

	//*************************************************************************
	// Method:		AddRange
	// Description: Adds a set of tree list nodes to the collection
	//
	// Parameters:
	//	items - the set of nodes to add
	//
	// Return Value: None
	//*************************************************************************
	void SelectedTreeListNodeCollection::AddRange(TreeListNode *items[])
	{
		System::Threading::Monitor::Enter(List->SyncRoot);
		try
		{
			for (int i = 0; i < items->Length; i++)
			{
				items[i]->Index = List->Add(items[i]);
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
	void SelectedTreeListNodeCollection::Remove(TreeListNode *item)
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
	void SelectedTreeListNodeCollection::ClearAll()
	{
		List->Clear();
	}
}