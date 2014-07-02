//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourcePaneDataNode.h
//
// DESCRIPTION: Contains definition for the class ResourcePaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 3 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourcePaneDataNode.h"
#include "ResourceTestDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ResourcePaneDataNode
	// Description: Constructor for the ResourcePaneDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	ResourcePaneDataNode::ResourcePaneDataNode(String *name, NodeType type, int pID, int tID)
	{
		InternalConstructor(name, type, pID, tID);
		numHits = 0;
		filePosition = -1;
		displayable = true;
	}

	//*************************************************************************
	// Method:		~ResourcePaneDataNode
	// Description: Destructor for the ResourcePaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourcePaneDataNode::~ResourcePaneDataNode()
	{
	}

	//*************************************************************************
	// Method:		ConvertToTextFormat
	// Description: Copies data from this datanode to into a formatted string
	//
	// Parameters:
	//	None
	//
	// Return Value: A formatted string containing data from this dataNode
	//*************************************************************************
	String* ResourcePaneDataNode::ConvertToTextFormat()
	{
		ResourceTestDataNode *resourceTestNode = dynamic_cast <ResourceTestDataNode*> (this->ResourceTestNode);
		String * text = S"";
		text = String::Concat (text, S"Timestamp: ", timeStamp, S"\r\n");
		text = String::Concat (text, S"Name: ", this->Name, S"\r\n");
		text = String::Concat (text, S"Description: ", this->ObjectDescription, S"\r\n");
		text = String::Concat (text, S"Access count: ", numHits.ToString(), S"\r\n");
		text = String::Concat (text, S"Last accessed by: ", lastFunction, S", which returned with value ", returnValue, S" and error code ", errorCode, S"\r\n");

		if (resourceTestNode)
		{
			text = String::Concat (text, S"Fault set: ", resourceTestNode->FaultName, "\r\n");
		}

		return text;
	}

	//*************************************************************************
	// Method:		ConvertIIDToName
	// Description: convert clsid to friendly name
	//
	// Parameters:
	//	objectString - clid string
	//
	// Return Value: description string if found otherwise return the string passed in
	//*************************************************************************
	String *ResourcePaneDataNode::ConvertIIDToName(String *objectString)
	{
		if (this->Type == UIData::ComObject)
		{
			RegistryKey *regKey;

			try
			{
				String *retValue;
				String *keyName = String::Concat("CLSID\\", objectString);
				regKey = Registry::ClassesRoot->OpenSubKey(keyName);

				if (regKey)
					retValue = regKey->GetValue(NULL)->ToString();
				
				if (!retValue || retValue->Length == 0)
					retValue = objectString;

				if (regKey)
					regKey->Close();

				return retValue;
			}
			catch(...)
			{
				if (regKey)
					regKey->Close();

				return objectString;
			}
		}
		else
			return "N/A";
	}
}