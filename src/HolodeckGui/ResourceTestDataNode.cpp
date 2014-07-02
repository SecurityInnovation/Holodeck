//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestDataNode.cpp
//
// DESCRIPTION: Contains definition for the class ResourceTestDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Sept 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ResourceTestDataNode
	// Description: Constructor for the ResourceTestDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	pID - the process id of the process this node is associated with
	//	tID - the thread id of the thread this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestDataNode::ResourceTestDataNode(String *resourceName, String *FaultName, FunctionDatabase::FaultType faultType, int pID, int tID) : 
		DisplayableDataNode(resourceName, UIData::NodeType::ResourceFault, pID, tID)
	{
		int faultIDNum;
		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();

		faultName = FaultName;
		this->faultType = faultType;
		if (faultName)
			faultIDNum = faultDB->GetFaultIDFromString(faultName, faultType);
		else
			faultIDNum = -1;

		if (faultIDNum != -1)
			faultID = Convert::ToString(faultIDNum);

		this->isTestOn = true;
	}

	//*************************************************************************
	// Method:		ResourceTestDataNode
	// Description: Copy constructor for the ResourceTestDataNode class
	//
	// Parameters:
	//	resourceTestNode - node to copy
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestDataNode::ResourceTestDataNode(ResourceTestDataNode *resourceTestNode) : 
		DisplayableDataNode(resourceTestNode)
	{
		testIDs = resourceTestNode->TestIDs;
		faultName = resourceTestNode->FaultName;
		faultID = resourceTestNode->FaultID;
		faultType = resourceTestNode->FaultType;
		isTestOn = resourceTestNode->IsTestOn;
	}

	//*************************************************************************
	// Method:		~ResourceTestDataNode
	// Description: Destructor for the ResourceTestDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestDataNode::~ResourceTestDataNode()
	{
	}
}