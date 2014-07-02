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
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace Microsoft::Win32;

namespace UIData
{
	public __gc class ResourcePaneDataNode : public DisplayableDataNode
	{
	public:
		[ReadOnly(true), Description("The last time this resource was accessed"), Category("Function")]
		__property String *get_TimeStamp() { return timeStamp; }
		__property void set_TimeStamp(String *value) { timeStamp = value; }
		[ReadOnly(true), Description("The last function called that accessed this resource"), Category("Function")]
		__property String *get_LastFunction() { return lastFunction; }
		__property void set_LastFunction(String *value) { lastFunction = value; }
		[ReadOnly(true), Description("The number of times this resource has been accessed"), Category("Function")]
		__property int get_NumHits() { return numHits; }
		__property void set_NumHits(int value) { numHits = value; }
		[ReadOnly(true), Description("The error code returned by the function that accessed this resource"), Category("Function")]
		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String *value) { errorCode = value; }
		[ReadOnly(true), Description("The return value of the function that accessed this resource"), Category("Function")]
		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String *value) { returnValue = value; }
		[ReadOnly(true), Description("The description of the resource, if available")]
		__property String *get_ObjectDescription() { return ConvertIIDToName(Name); }
		[Browsable(false)]
		__property bool get_Displayable() { return displayable; }
		__property void set_Displayable(bool value) { displayable = value; }
		[Browsable(false)]
		__property DisplayableDataNode *get_ResourceTestNode() { return resourceTestNode; }
		__property void set_ResourceTestNode(DisplayableDataNode *value) 
		{ 
			resourceTestNode = value; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "ResourceTest");
		}
		[ReadOnly(true), Description("True if a test is associated with this resource"), Category("Function")]
		__property bool get_HasResourceTest() 
		{
			if (resourceTestNode)
				return true;
			else
				return false;
		}

		[Browsable(false)]
		__property String *get_ClipboardText() { return ConvertToTextFormat(); }

		[Browsable(false)]
		__property unsigned long get_FilePosition() { return filePosition; }
		__property void set_FilePosition(unsigned long value) { filePosition = value; }

		ResourcePaneDataNode(String *name, NodeType type, int pID, int tID);
		~ResourcePaneDataNode();

	private:
		int numHits;
		bool displayable;
		String *timeStamp;
		String *lastFunction;
		String *errorCode;
		String *returnValue;
		DisplayableDataNode *resourceTestNode;
		unsigned long filePosition;

		String * ConvertToTextFormat();
		String *ConvertIIDToName(String *objectString);
	};
}