//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionDataNode.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "NetworkCorruptionDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		NetworkCorruptionDataNode
	// Description: Constructor for the NetworkCorruptionDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	pID - the process id of the process this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionDataNode::NetworkCorruptionDataNode(String *Name, int pID) : 
		DisplayableDataNode(Name, UIData::NodeType::NetworkCorruptionFault, pID, 0)
	{
		this->isTestOn = true;
	}

	//*************************************************************************
	// Method:		NetworkCorruptionDataNode
	// Description: Copy constructor for the NetworkCorruptionDataNode class
	//
	// Parameters:
	//	ncNode - node to copy
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionDataNode::NetworkCorruptionDataNode(NetworkCorruptionDataNode *ncNode) : 
		DisplayableDataNode(ncNode)
	{
		sentData = ncNode->SentData;
		receivedData = ncNode->ReceivedData;
		portList = ncNode->PortList;
		portStr = ncNode->PortString;
		corruptType = ncNode->CorruptType;
		searchFor = ncNode->SearchFor;
		searchRegExpr = ncNode->SearchRegularExpression;
		replaceWith = ncNode->ReplaceWith;
		replacementStr = ncNode->ReplacementString;
		uiSettings = ncNode->UISettings;
		isTestOn = ncNode->IsTestOn;
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionDataNode
	// Description: Destructor for the NetworkCorruptionDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionDataNode::~NetworkCorruptionDataNode()
	{
	}

	//*************************************************************************
	// Method:		GetPortListFromPortString
	// Description: Calculates the port list from the port string
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionDataNode::GetPortListFromPortString()
	{
		// Formats of port string:
		// Send on xxx
		// Recv on xxx
		// Send or Recv on xxx

		// Formats of port list within string:
		// 1, 2, 3, ...
		// All
		// All Except 1, 2, 3, ...

		int index = PortString->IndexOf("on");
		String* portListStr = PortString->Substring(index + 3); // After "on" in string

		PortList = new ArrayList();
		if (portListStr->StartsWith("All Except"))
		{
			// Start with all ports enabled
			for (int i=1; i<65536; i++)
				PortList->Add(__box(i));

			// Exclude list
			portListStr = portListStr->Substring(11); // Skip past "All Except "
			while (portListStr->Length > 0)
			{
				index = portListStr->IndexOf(",");
				String* numStr;
				if (index != -1)
					numStr = portListStr->Substring(0, index);
				else
					numStr = portListStr;

				int dashIndex = numStr->IndexOf("-");
				if (dashIndex != -1)
				{
					String* endStr = numStr->Substring(dashIndex + 1);
					numStr = numStr->Substring(0, dashIndex);
					int begin = Convert::ToInt32(numStr);
					int end = Convert::ToInt32(endStr);
					for (int i=begin; i<=end; i++)
						PortList->Remove(__box(i));
				}
				else
					PortList->Remove(__box(Convert::ToInt32(numStr)));

				if (index != -1)
					portListStr = portListStr->Substring(index + 2); // skip past ", "
				else
					break;
			}
		}
		else if (portListStr->StartsWith("All"))
		{
			// All ports enabled
			for (int i=1; i<65536; i++)
				PortList->Add(__box(i));
		}
		else
		{
			// Include list
			while (portListStr->Length > 0)
			{
				index = portListStr->IndexOf(",");
				String* numStr;
				if (index != -1)
					numStr = portListStr->Substring(0, index);
				else
					numStr = portListStr;

				int dashIndex = numStr->IndexOf("-");
				if (dashIndex != -1)
				{
					String* endStr = numStr->Substring(dashIndex + 1);
					numStr = numStr->Substring(0, dashIndex);
					int begin = Convert::ToInt32(numStr);
					int end = Convert::ToInt32(endStr);
					for (int i=begin; i<=end; i++)
						PortList->Add(__box(i));
				}
				else
					PortList->Add(__box(Convert::ToInt32(numStr)));

				if (index != -1)
					portListStr = portListStr->Substring(index + 2); // skip past ", "
				else
					break;
			}
		}
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
	String * NetworkCorruptionDataNode::ConvertToTextFormat()
	{
		String * Text = "";
		Text = String::Concat (Text, S"Test On: ", this->isTestOn ? "True":"False", S"\r\n");
		Text = String::Concat (Text, S"Ports: ", portStr, S"\r\n");
		Text = String::Concat (Text, S"Network Fuzzing Type: ", corruptType, S"\r\n");
		if (!corruptType->Equals("RegExpr"))
		{
			Text = String::Concat (Text, S"Search For: ", searchFor, S"\r\n");
			Text = String::Concat (Text, S"Replace With: ", replaceWith, S"\r\n");
		}
		else
		{
			Text = String::Concat (Text, S"Regular Expression: ", searchRegExpr, S"\r\n");
			Text = String::Concat (Text, S"Expression Replacement: ", replacementStr, S"\r\n");
		}
		Text = String::Concat (Text, S"Network Fuzzing Type: ", corruptType, S"\r\n");

		return Text;
	}
}
