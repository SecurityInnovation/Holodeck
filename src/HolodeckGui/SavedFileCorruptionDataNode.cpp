//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionDataNode.cpp
//
// DESCRIPTION: Contains definition for the class SavedFileCorruptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#include "SavedFileCorruptionDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		SavedFileCorruptionDataNode
	// Description: Constructor for the SavedFileCorruptionDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	type - SavedCorruptedFile or CorruptedFile
	//	pID - the process id of the process this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionDataNode::SavedFileCorruptionDataNode(String *Name, NodeType type, int pID) : 
		DisplayableDataNode(Name, type, pID, 0)
	{
	}

	//*************************************************************************
	// Method:		~SavedFileCorruptionDataNode
	// Description: Destructor for the SavedFileCorruptionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionDataNode::~SavedFileCorruptionDataNode()
	{
	}
}