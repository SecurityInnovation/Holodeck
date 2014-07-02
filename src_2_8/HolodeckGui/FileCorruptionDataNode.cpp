//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDataNode.cpp
//
// DESCRIPTION: Contains definition for the class FileCorruptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "FileCorruptionDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		FileCorruptionDataNode
	// Description: Constructor for the FileCorruptionDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	pID - the process id of the process this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDataNode::FileCorruptionDataNode(String *Name, int pID) : 
		DisplayableDataNode(Name, UIData::NodeType::FileCorruptionFault, pID, 0)
	{
		this->isTestOn = true;
	}

	//*************************************************************************
	// Method:		FileCorruptionDataNode
	// Description: copy Constructor for the FileCorruptionDataNode class
	//
	// Parameters:
	//	fcNode - node to copy
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDataNode::FileCorruptionDataNode(FileCorruptionDataNode *fcNode) : 
		DisplayableDataNode(fcNode)
	{
		origFile = fcNode->OriginalFile;
		corruptType = fcNode->CorruptType;
		searchFor = fcNode->SearchFor;
		regen = fcNode->Regenerate;
		searchRegExpr = fcNode->SearchRegularExpression;
		replaceWith = fcNode->ReplaceWith;
		replacementStr = fcNode->ReplacementString;
		uiSettings = fcNode->UISettings;
		isTestOn = fcNode->IsTestOn;
	}

	//*************************************************************************
	// Method:		~FileCorruptionDataNode
	// Description: Destructor for the FileCorruptionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDataNode::~FileCorruptionDataNode()
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
	String * FileCorruptionDataNode::ConvertToTextFormat()
	{
		String * Text = "";
		Text = String::Concat (Text, S"Test On: ", this->isTestOn ? "True":"False", S"\r\n");
		Text = String::Concat (Text, S"File to fuzz: ", origFile, S"\r\n");
		Text = String::Concat (Text, S"File Fuzzing Type: ", corruptType, S"\r\n");
		if (!corruptType->Equals("RegExpr"))
		{
			Text = String::Concat (Text, S"Search For: ", searchFor, S"\r\n");
			Text = String::Concat (Text, S"Replace With: ", replaceWith, S"\r\n");
		}
		else
		{
			Text = String::Concat (Text, S"Search Regular Expression: ", searchRegExpr, S"\r\n");
			Text = String::Concat (Text, S"Expression Replacement: ", replacementStr, S"\r\n");
		}
		Text = String::Concat (Text, S"New fuzzing everytime file is accessed: ", regen? S"Yes":S"No", S"\r\n");
		return Text;
	}
}