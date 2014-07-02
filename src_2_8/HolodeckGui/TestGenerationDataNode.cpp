//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenerationDataNode.h
//
// DESCRIPTION: Contains definition for the class TestGenerationDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Feb 2003		 J. Taylor	 File created.
//*************************************************************************
#include "TestGenerationDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		TestGenerationDataNode
	// Description: Constructor for the TestGenerationDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	TestGenerationDataNode::TestGenerationDataNode(DisplayableDataNode *DataNode)
	{
		dataNode = DataNode;
		logIndex = 0;
		InternalConstructor("", Process, 0, 0);
	}

	//*************************************************************************
	// Method:		~RecordedSessionDataNode
	// Description: Destructor for the RecordedSessionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestGenerationDataNode::~TestGenerationDataNode()
	{
	}
}