//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class NetworkLogPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#include "NetworkLogPaneDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		NetworkLogPaneDataNode
	// Description: Constructor for the NetworkLogPaneDataNode class
	//
	// Parameters:
	//	item - The network log item that this data node encapsulates
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataNode::NetworkLogPaneDataNode(NetworkLogItem *item)
	{
		this->logItem = item;
		this->Type = NetworkLogs;
	}

	//*************************************************************************
	// Method:		~NetworkLogPaneDataNode
	// Description: Destructor for the NetworkLogPaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataNode::~NetworkLogPaneDataNode()
	{
	}
}