//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class FaultPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 4 Mar  2004		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;

namespace UIData
{
	public __gc class FaultPaneDataNode : public DisplayableDataNode
	{
	protected:
		int faultID;

	public:
		FaultPaneDataNode(String *Name, NodeType Type, int pID, int tID);
		FaultPaneDataNode(FaultPaneDataNode *faultNode);
		~FaultPaneDataNode();

		[Browsable(false)]
		__property int get_FaultID() { return faultID; }
		__property void set_FaultID(int value) { faultID = value; }
	};
}