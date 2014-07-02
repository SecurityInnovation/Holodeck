//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListNodeCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class TreeListNodeCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "TreeListNodeCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		TreeListNodeCollection
	// Description: Constructor for the TreeListNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	//TreeListNodeCollection::TreeListNodeCollection()
	//{
	//}

	//*************************************************************************
	// Method:		TreeListNodeCollection
	// Description: Constructor for the TreeListNodeCollection class
	//
	// Parameters:
	//	owner - the owner of this collection
	//
	// Return Value: None
	//*************************************************************************
	TreeListNodeCollection::TreeListNodeCollection(TreeListNode *owner)
	{
		this->owner = owner;
	}

	//*************************************************************************
	// Method:		~TreeListNodeCollection
	// Description: Destructor for the TreeListNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TreeListNodeCollection::~TreeListNodeCollection()
	{
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: returns the item at the specified index
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: the node at the specified index
	//*************************************************************************
	TreeListNode *TreeListNodeCollection::get_Item(int index)
	{
//		if (QueryTreeListNode)
//			return QueryTreeListNode->Invoke(index);
		return NULL;
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
	int TreeListNodeCollection::IndexOf(TreeListNode *item)
	{
//		if (QueryTreeListNodeIndex)
//			return QueryTreeListNodeIndex->Invoke(item);
		return 0;
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: invokes the MouseDown delegate for the class
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNodeCollection::OnMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(sender, e);
	}

	//*************************************************************************
	// Method:		OnNodesChanged
	// Description: invokes the NodesChanged delegate for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNodeCollection::OnNodesChanged()
	{
		OnNodesChanged(this, new EventArgs());
	}

	//*************************************************************************
	// Method:		OnNodesChanged
	// Description: invokes the NodesChanged delegate for the class
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the changed event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNodeCollection::OnNodesChanged(Object *sender, EventArgs *e)
	{
		if (NodesChanged)
			NodesChanged->Invoke(sender, e);
	}
}