//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphNode.cpp
//
// DESCRIPTION: Contains implemenation for the class GraphNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 23 Jul 2004		 R. Wagner	  File created.
//*************************************************************************
#include <windows.h>
#include "GraphNode.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		GraphNode
	// Description: Constructor for the GraphNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphNode::GraphNode()
	{
		items = new ArrayList();
		backColor = Color::Empty;
		foreColor = Color::Empty;
	}

	//*************************************************************************
	// Method:		~GraphNode
	// Description: Destructor for the GraphNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphNode::~GraphNode()
	{
	}
}
