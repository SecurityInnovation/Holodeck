//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenerationDataNode.h
//
// DESCRIPTION: Contains definition for the class TestGenerationDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Feb 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace HoloScript;

namespace UIData
{


	public __gc class TestGenerationDataNode : public DisplayableDataNode
	{
	public:
		__property void set_LogIndex(unsigned int value) { logIndex = value; }
		__property unsigned int get_LogIndex() { return logIndex; }

		__property DisplayableDataNode *get_DataNode() { return dataNode; }

		TestGenerationDataNode(DisplayableDataNode *dataNode);
		~TestGenerationDataNode();

	private:
		unsigned int logIndex;
		DisplayableDataNode *dataNode;
	};
}