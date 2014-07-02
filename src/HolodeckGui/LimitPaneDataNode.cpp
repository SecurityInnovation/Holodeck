//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class LimitPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "LimitPaneDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		LimitPaneDataNode
	// Description: Constructor for the LimitPaneDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	LimitPaneDataNode::LimitPaneDataNode(String *Name, NodeType Type, int pID, int tID)
	{
		InternalConstructor(Name, Type, pID, tID);
	}

	//*************************************************************************
	// Method:		LimitPaneDataNode
	// Description: Copy constructor for the LimitPaneDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	LimitPaneDataNode::LimitPaneDataNode(LimitPaneDataNode *limitNode):
		DisplayableDataNode(limitNode)
	{
		limitValue = limitNode->LimitValue;
	}

	//*************************************************************************
	// Method:		~LimitPaneDataNode
	// Description: Destructor for the LimitPaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LimitPaneDataNode::~LimitPaneDataNode()
	{
	}
}