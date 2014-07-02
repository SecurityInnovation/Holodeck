//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphNodeCollection.cpp
//
// DESCRIPTION: Contains implemenation for the class GraphNodeCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 23 Jul 2004		 R. Wagner	  File created.
//*************************************************************************
#include <windows.h>
#include "GraphNodeCollection.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		GraphNodeCollection
	// Description: Constructor for the GraphNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphNodeCollection::GraphNodeCollection()
	{
		nodeTable = new Hashtable();
	}

	//*************************************************************************
	// Method:		~GraphNodeCollection
	// Description: Destructor for the GraphNodeCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphNodeCollection::~GraphNodeCollection()
	{
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: Gets a node by ID
	//
	// Parameters:
	//	id - Node ID
	//
	// Return Value: Node object
	//*************************************************************************
	GraphNode* GraphNodeCollection::get_Item(Object* id)
	{
		return dynamic_cast<GraphNode*>(nodeTable->Item[id]);
	}

	//*************************************************************************
	// Method:		get_Item
	// Description: Gets a node by ID
	//
	// Parameters:
	//	id - Node ID
	//
	// Return Value: Node object
	//*************************************************************************
	void GraphNodeCollection::set_Item(Object* id, GraphNode* node)
	{
		if (!id->Equals(node->NodeId))
		{
			if (nodeTable->ContainsKey(node))
				Remove(node);
			node->NodeId = id;
		}
		Add(node);
	}

	//*************************************************************************
	// Method:		get_NodeList
	// Description: Gets the list of nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: List of nodes
	//*************************************************************************
	ICollection* GraphNodeCollection::get_NodeList()
	{
		return nodeTable->Values;
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a node
	//
	// Parameters:
	//	node - Node to add
	//
	// Return Value: None
	//*************************************************************************
	void GraphNodeCollection::Add(GraphNode* node)
	{
		if (!node)
			return;

		if (nodeTable->ContainsKey(node->NodeId))
			Remove(dynamic_cast<GraphNode*>(nodeTable->Item[node->NodeId]));

		nodeTable->Item[node->NodeId] = node;
		if (OnGraphNodeAdded != NULL)
			OnGraphNodeAdded->Invoke(node);
	}

	//*************************************************************************
	// Method:		Remove
	// Description: Removes a node
	//
	// Parameters:
	//	node - Node to remove
	//
	// Return Value: None
	//*************************************************************************
	void GraphNodeCollection::Remove(GraphNode* node)
	{
		if (!node)
			return;

		if (nodeTable->ContainsKey(node->NodeId) && (OnGraphNodeRemoved != NULL))
		{
			nodeTable->Remove(node);
			OnGraphNodeRemoved->Invoke(node);
		}
	}
}
