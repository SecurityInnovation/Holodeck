//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class LimitPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class LimitPaneDataNode : public DisplayableDataNode
	{
	public:
		[ReadOnly(true), Description("The value set for this limit"), Category("Owner Info")]
		__property UInt64 get_LimitValue() { return limitValue; }
		__property void set_LimitValue(UInt64 value) { limitValue = value; }

		LimitPaneDataNode(String *Name, NodeType Type, int pID, int tID);
		LimitPaneDataNode(LimitPaneDataNode *limitNode);
		~LimitPaneDataNode();

	private:
		UInt64 limitValue;
	};
}