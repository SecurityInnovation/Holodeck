//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDataNode.h
//
// DESCRIPTION: Contains definition for the class ExceptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#include "ExceptionDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ExceptionDataNode
	// Description: Constructor for the ExceptionDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	pID - the process id of the process this node is associated with
	//	tID - the thread id of the thread this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDataNode::ExceptionDataNode(String *Name, int pID, int tID) : 
		DisplayableDataNode(Name, UIData::NodeType::ExceptionInfo, pID, tID)
	{
	}

	//*************************************************************************
	// Method:		~ExceptionDataNode
	// Description: Destructor for the ExceptionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDataNode::~ExceptionDataNode()
	{
	}

	//*************************************************************************
	// Method:		ConvertToTextFormat
	// Description: Converts the object to a string, used for pasting item into
	//				the clipboard
	//
	// Parameters:
	//	None
	//
	// Return Value: string that represents the object
	//*************************************************************************	
	String * ExceptionDataNode::ConvertToTextFormat()
	{
		String * Text = "";
		Text = String::Concat (Text, S"Executable name: ", ExeName, S"\r\n");
		Text = String::Concat (Text, S"Time stamp: ", TimeStamp, S"\r\n");
		Text = String::Concat (Text, S"Exception: ", Exception, S"\r\n");
		Text = String::Concat (Text, S"Mini Dump File: ", MiniDumpFile, S"\r\n");
		return Text;
	}
}