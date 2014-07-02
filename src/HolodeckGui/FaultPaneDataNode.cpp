//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class FaultPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 4 Mar  2004		 J. Taylor	 File created.
//*************************************************************************
#include "FaultPaneDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		FaultPaneDataNode
	// Description: Constructor for the FaultPaneDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	Type - fault type
	//	pID - the process id of the process this node is associated with
	//	tID - the thread id of the thread this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	FaultPaneDataNode::FaultPaneDataNode(String *Name, NodeType Type, int pID, int tID) : 
		DisplayableDataNode(Name, Type, pID, tID)
	{
		faultID = -1;
	}

	//*************************************************************************
	// Method:		FaultPaneDataNode
	// Description: copy Constructor for the FaultPaneDataNode class
	//
	// Parameters:
	//	faultNode - node to copy
	//
	// Return Value: None
	//*************************************************************************
	FaultPaneDataNode::FaultPaneDataNode(FaultPaneDataNode *faultNode) : 
		DisplayableDataNode(faultNode)
	{
		faultID = faultNode->FaultID;
	}

	//*************************************************************************
	// Method:		~FaultPaneDataNode
	// Description: Destructor for the FaultPaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultPaneDataNode::~FaultPaneDataNode()
	{
	}
}