//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewItemCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class ContainerListViewItemCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ContainerListViewItemCollection.h"
#include "ContainerListViewItem.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ContainerListViewItemCollection
	// Description: Constructor for the ContainerListViewItemCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewItemCollection::ContainerListViewItemCollection()
	{
		itemCount = 0;
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: returns the ContainerListViewItem at the specified index
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: the ContainerListViewItem at the specified index
	//*************************************************************************
	ContainerListViewItem *ContainerListViewItemCollection::get_Item(int index)
	{
		if (QueryListViewItem)
			return QueryListViewItem->Invoke(index);

		return NULL;
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
	int ContainerListViewItemCollection::IndexOf(ContainerListViewItem *item)
	{
		if (QueryListViewItemIndex)
			return QueryListViewItemIndex->Invoke(item);

		return 0;
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: invokes the mouse down delegate
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewItemCollection::OnMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(sender, e);
	}
}